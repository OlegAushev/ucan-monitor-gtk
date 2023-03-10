#include "client.h"


namespace ucanopen {

Client::Client(NodeId node_id, std::shared_ptr<can::Socket> socket)
	: _node_id(node_id)
	, _socket(socket)
	, _nmt_state(NmtState::initialization)
{
	_sync_info.timepoint = std::chrono::steady_clock::now();
	_heartbeat_info.timepoint = std::chrono::steady_clock::now();

	Log() << "[ucanopen] Starting aux thread...\n";

	std::future<void> signal_exit = _signal_exit_run_thread.get_future();
	_thread_run = std::thread(&Client::_run, this, std::move(signal_exit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	_nmt_state = NmtState::operational;
}


Client::~Client()
{
	Log() << "[ucanopen] Sending signal to aux thread to stop...\n";
	_signal_exit_run_thread.set_value();
	_thread_run.join();
}


void Client::set_node_id(NodeId node_id)
{
	Log() << "[ucanopen] Setting client ID = " << node_id.get()
			<< " (0x" << std::hex << node_id.get() << std::dec << ")... ";

	if (!node_id.is_valid())
	{
		Log() << "failed: invalid ID.\n";
		return;
	}

	if (!_is_free(node_id))
	{
		Log() << "failed: already occupied ID.\n";
		return;
	}
	
	_node_id = node_id;
	Log() << "done.\n";
}


void Client::register_server(std::shared_ptr<Server> server)
{
	Log() << "[ucanopen] Adding '" << server->name() << "' server ID 0x" 
			<< std::hex << server->node_id().get() << std::dec << " to client... ";

	auto server_same_name = std::find_if(_servers.begin(), _servers.end(), 
		[server](const auto& s)
		{
			return server->name() == s->name();				
		});
	if (server_same_name != _servers.end())
	{
		Log() << "failed: server with that name already added to client.\n";
		return;
	}

	auto server_same_id = std::find_if(_servers.begin(), _servers.end(), 
		[server](const auto& s)
		{
			return server->node_id() == s->node_id();				
		});
	if (server_same_id != _servers.end())
	{
		Log() << "failed: server with ID 0x" << std::hex << server->node_id().get() << std::dec
				<< " already added to client.\n";
		return;
	}

	if (server->node_id() == _node_id)
	{
		Log() << "failed: client has the same ID 0x" << std::hex << server->node_id().get() << std::dec << '\n';
		return;
	}

	_servers.insert(server);
	_calculate_recvid(server);

	Log() << "done.\n";
}


void Client::set_server_node_id(std::string_view name, NodeId node_id)
{
	Log() << "[ucanopen] Setting '" << name << "' server ID = " << node_id.get()
				<< " (0x" << std::hex << node_id.get() << std::dec << ")... ";

	if (!node_id.is_valid())
	{
		Log() << "failed: invalid ID.\n";
		return;
	}

	if (!_is_free(node_id))
	{
		Log() << "failed: already occupied ID.\n";
		return;
	}

	auto server_iter = std::find_if(_servers.begin(), _servers.end(),
		[name](const auto& s)
		{
			return s->name() == name;
		});
	if (server_iter == _servers.end())
	{
		Log() << "failed: no such server found.\n";
		return;
	}

	(*server_iter)->_set_node_id(node_id);

	// erase outdated elements from [id; server] map
	for (auto it = _recvid_server_list.begin(); it != _recvid_server_list.end();)
	{
		if (it->second->name() == name)
		{
			it = _recvid_server_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	_calculate_recvid(*server_iter);

	Log() << "done.\n";
}


void Client::_run(std::future<void> signal_exit)
{
	Log() << "[ucanopen] Aux thread started. Thread id: " << std::this_thread::get_id() << '\n';

	while (signal_exit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* SYNC */
		if (_sync_info.is_enabled)
		{
			if (now - _sync_info.timepoint > _sync_info.period)
			{
				_socket->send(create_frame(CobType::sync, _node_id, {}));
				_sync_info.timepoint = now;
			}
		}

		/* HEARTBEAT */
		if (now - _heartbeat_info.timepoint > _heartbeat_info.period)
		{
			_socket->send(create_frame(CobType::heartbeat, _node_id, {static_cast<uint8_t>(_nmt_state)}));
			_heartbeat_info.timepoint = now;
		}

		/* TPDO */
		if (_is_tpdo_enabled)
		{
			for (auto& [tpdo_type, message] : _tpdo_list)
			{
				if (!message.creator) continue;
				if (now - message.timepoint >= message.period)
				{
					_socket->send(create_frame(to_cob_type(tpdo_type), _node_id, message.creator()));
					message.timepoint = now;
				}
			}
		}

		/* SEND TO SERVERS */
		for (auto& server : _servers)
		{
			server->_send();
		}

		/* RECV */
		can_frame frame;
		can::Error recv_error = _socket->recv(frame);
		while (recv_error == can::Error::none)
		{
			(void) std::async(&Client::_on_frame_received, this, frame);
			recv_error = _socket->recv(frame);
		}
	}

	Log() << "[ucanopen] Aux thread stopped.\n";
}


void Client::_on_frame_received(const can_frame& frame)
{
	auto server_iter = _recvid_server_list.find(frame.can_id);
	if (server_iter != _recvid_server_list.end())
	{
		server_iter->second->_handle_frame(frame);
	}
}


void Client::_calculate_recvid(std::shared_ptr<Server> server)
{
	canid_t tpdo1 = calculate_cob_id(CobType::tpdo1, server->node_id());
	canid_t tpdo2 = calculate_cob_id(CobType::tpdo2, server->node_id());
	canid_t tpdo3 = calculate_cob_id(CobType::tpdo3, server->node_id());
	canid_t tpdo4 = calculate_cob_id(CobType::tpdo4, server->node_id());
	canid_t tsdo = calculate_cob_id(CobType::tsdo, server->node_id());
	canid_t heartbeat = calculate_cob_id(CobType::heartbeat, server->node_id());

	_recvid_server_list.insert({tpdo1, server});
	_recvid_server_list.insert({tpdo2, server});
	_recvid_server_list.insert({tpdo3, server});
	_recvid_server_list.insert({tpdo4, server});
	_recvid_server_list.insert({tsdo, server});
	_recvid_server_list.insert({heartbeat, server});
}


bool Client::_is_free(NodeId node_id) const
{
	if (node_id == _node_id)
	{
		return false;
	}

	for (const auto& server : _servers)
	{
		if (node_id == server->node_id())
		{
			return false;
		}
	}

	return true;
}

} // namespace ucanopen

