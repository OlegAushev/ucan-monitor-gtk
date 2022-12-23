/**
 * @file ucanopen_client.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen_client.h"


namespace ucanopen {


///
///
///
Client::Client(NodeId nodeId, std::shared_ptr<can::Socket> socket)
	: _nodeId(nodeId)
	, _socket(socket)
	, _state(NmtState::Initialization)
{
	_syncInfo.timepoint = std::chrono::steady_clock::now();
	_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	std::cout << "[ucanopen] Starting aux thread..." << std::endl;

	std::future<void> futureExit = _signalExitRunThread.get_future();
	_threadRun = std::thread(&Client::_run, this, std::move(futureExit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	_state = NmtState::Operational;
}


///
///
///
Client::~Client()
{
	std::cout << "[ucanopen] Sending signal to aux thread to stop..." << std::endl;
	_signalExitRunThread.set_value();
	_threadRun.join();
}


///
///
///
void Client::setNodeId(NodeId nodeId)
{
	std::cout << "[ucanopen] Setting client ID = " << nodeId.value()
			<< " (0x" << std::hex << nodeId.value() << std::dec << ")... ";

	if (!nodeId.isValid())
	{
		std::cout << "failed: invalid ID." << std::endl;
		return;
	}

	if (!_isFree(nodeId))
	{
		std::cout << "failed: already occupied ID." << std::endl;
		return;
	}
	
	_nodeId = nodeId;
	std::cout << "done." << std::endl;
}


///
///
///
void Client::registerServer(std::shared_ptr<IServer> server)
{
	std::cout << "[ucanopen] Adding '" << server->name() << "' server ID 0x" 
			<< std::hex << server->nodeId().value() << std::dec << " to client... ";

	auto itServerSameName = std::find_if(_servers.begin(), _servers.end(), 
		[server](const auto& s)
		{
			return server->name() == s->name();				
		});
	if (itServerSameName != _servers.end())
	{
		std::cout << "failed: server with that name already added to client." << std::endl;
		return;
	}

	auto itServerSameId = std::find_if(_servers.begin(), _servers.end(), 
		[server](const auto& s)
		{
			return server->nodeId() == s->nodeId();				
		});
	if (itServerSameId != _servers.end())
	{
		std::cout << "failed: server with ID 0x" << std::hex << server->nodeId().value() << std::dec
				<< " already added to client."  << std::endl;
		return;
	}

	if (server->nodeId() == _nodeId)
	{
		std::cout << "failed: client has the same ID 0x" << std::hex << server->nodeId().value() << std::dec << std::endl;
		return;
	}

	_servers.insert(server);
	_calculateRecvId(server);

	std::cout << "done." << std::endl;
}


///
///
///
void Client::setServerNodeId(std::string_view name, NodeId nodeId)
{
	std::cout << "[ucanopen] Setting '" << name << "' server ID = " << nodeId.value()
				<< " (0x" << std::hex << nodeId.value() << std::dec << ")... ";

	if (!nodeId.isValid())
	{
		std::cout << "failed: invalid ID." << std::endl;
		return;
	}

	if (!_isFree(nodeId))
	{
		std::cout << "failed: already occupied ID." << std::endl;
		return;
	}

	auto itServer = std::find_if(_servers.begin(), _servers.end(),
		[name](const auto& s)
		{
			return s->name() == name;
		});
	if (itServer == _servers.end())
	{
		std::cout << "failed: no such server found." << std::endl;
		return;
	}

	(*itServer)->_setNodeId(nodeId);

	// erase outdated elements from [id; server] map
	for (auto it = _recvIdServerList.begin(); it != _recvIdServerList.end();)
	{
		if (it->second->name() == name)
		{
			it = _recvIdServerList.erase(it);
		}
		else
		{
			++it;
		}
	}

	_calculateRecvId(*itServer);

	std::cout << "done." << std::endl;
}


///
///
///
void Client::_run(std::future<void> futureExit)
{
	std::cout << "[ucanopen] Aux thread started. Thread id: " << std::this_thread::get_id() << std::endl;

	while (futureExit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* SYNC */
		if (_syncInfo.period != std::chrono::milliseconds(0))
		{
			if (now - _syncInfo.timepoint > _syncInfo.period)
			{
				_socket->send(createFrame(CobType::Sync, _nodeId, {}));
				_syncInfo.timepoint = now;
			}
		}

		/* HEARTBEAT */
		if (now - _heartbeatInfo.timepoint > _heartbeatInfo.period)
		{
			_socket->send(createFrame(CobType::Heartbeat, _nodeId, {static_cast<uint8_t>(_state)}));
			_heartbeatInfo.timepoint = now;
		}

		/* TPDO */
		if (_isTpdoEnabled)
		{
			for (auto& [type, message] : _tpdoList)
			{
				if (!message.creator) continue;
				if (now - message.timepoint >= message.period)
				{
					_socket->send(createFrame(toCobType(type), _nodeId, message.creator()));
					message.timepoint = now;
				}
			}
		}

		/* SERVER's RPDO */
		for (auto& server : _servers)
		{
			server->_sendPeriodic();
		}

		/* RECV */
		can_frame frame;
		can::Error recvErr = _socket->recv(frame);
		while (recvErr == can::Error::NoError)
		{
			(void) std::async(&Client::_onFrameReceived, this, frame);
			recvErr = _socket->recv(frame);
		}
	}

	std::cout << "[ucanopen] Aux thread stopped." << std::endl;
}


///
///
///
void Client::_onFrameReceived(const can_frame& frame)
{
	auto it = _recvIdServerList.find(frame.can_id);
	if (it != _recvIdServerList.end())
	{
		it->second->_handleFrame(frame);
	}
}


///
///
///
void Client::_calculateRecvId(std::shared_ptr<IServer> server)
{
	canid_t tpdo1 = calculateCobId(CobType::Tpdo1, server->nodeId());
	canid_t tpdo2 = calculateCobId(CobType::Tpdo2, server->nodeId());
	canid_t tpdo3 = calculateCobId(CobType::Tpdo3, server->nodeId());
	canid_t tpdo4 = calculateCobId(CobType::Tpdo4, server->nodeId());
	canid_t tsdo = calculateCobId(CobType::Tsdo, server->nodeId());
	canid_t heartbeat = calculateCobId(CobType::Heartbeat, server->nodeId());

	_recvIdServerList.insert({tpdo1, server});
	_recvIdServerList.insert({tpdo2, server});
	_recvIdServerList.insert({tpdo3, server});
	_recvIdServerList.insert({tpdo4, server});
	_recvIdServerList.insert({tsdo, server});
	_recvIdServerList.insert({heartbeat, server});
}


///
///
///
bool Client::_isFree(NodeId nodeId) const
{
	if (nodeId == _nodeId)
	{
		return false;
	}

	for (const auto& server : _servers)
	{
		if (nodeId == server->nodeId())
		{
			return false;
		}
	}

	return true;
}


} // namespace ucanopen


