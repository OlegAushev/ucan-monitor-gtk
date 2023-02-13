#pragma once


#include "../ucanopen_def.h"
#include "../server/ucanopen_server.h"
#include <cansocket/cansocket.h>
#include <sstream>
#include <iomanip>
#include <functional>
#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include <map>

#include <log/log.h>


namespace ucanopen {

class Client
{
private:
	NodeId _node_id;
	std::shared_ptr<can::Socket> _socket;
	NmtState _nmt_state;

	std::set<std::shared_ptr<Server>> _servers;
	std::map<canid_t, std::shared_ptr<Server>> _recvid_server_list;

	/* SYNC */
	struct SyncInfo
	{
		bool is_enabled = false;
		std::chrono::milliseconds period = std::chrono::milliseconds(1000);
		std::chrono::time_point<std::chrono::steady_clock> timepoint;		
	};
	SyncInfo _sync_info;

	/* HEARTBEAT */
	struct HeartbeatInfo
	{
		std::chrono::milliseconds period = std::chrono::milliseconds(1000);
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	HeartbeatInfo _heartbeat_info;
			
	/* TPDO client --> server */
	bool _is_tpdo_enabled = false;
	struct TpdoInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload(void)> creator;
	};
	std::map<TpdoType, TpdoInfo> _tpdo_list;

	/* THREADS */
	std::thread _thread_run;
	std::promise<void> _signal_exit_run_thread;
	void _run(std::future<void> future_exit);

	void _on_frame_received(const can_frame& frame);

public:
	Client(NodeId node_id, std::shared_ptr<can::Socket> socket);
	~Client();
	NodeId node_id() const { return _node_id; }
	void set_node_id(NodeId nodeId);
	void register_server(std::shared_ptr<Server> server);

	std::shared_ptr<Server> server(std::string_view name)
	{
		auto server_iter = std::find_if(_servers.begin(), _servers.end(),
			[name](const auto& s)
			{
				return s->name() == name;
			});
		if (server_iter == _servers.end())
		{
			return nullptr;
		}
		return *server_iter;
	}

	void set_server_node_id(std::string_view name, NodeId node_id);

	void enable_sync()
	{
		Log() << "[ucanopen] Enabling client SYNC messages (period = " << _sync_info.period << ")... ";	
		_sync_info.is_enabled = true;
		Log() << "done.\n";
	}

	void disable_sync()
	{
		Log() << "[ucanopen] Disabling client SYNC messages... ";
		_sync_info.is_enabled = false;
		Log() << "done.\n";
	}

	void set_sync_period(std::chrono::milliseconds period)
	{
		Log() << "[ucanopen] Setting client SYNC messages period = " << period << "... ";
		_sync_info.period = period;
		Log() << "done.\n";
	}

	void set_heartbeat_period(std::chrono::milliseconds period)
	{
		_heartbeat_info.period = period;
	}

	void register_tpdo(TpdoType tpdo_type, std::chrono::milliseconds period, std::function<can_payload(void)> creator)
	{
		_tpdo_list.insert({tpdo_type, {period, std::chrono::steady_clock::now(), creator}});
	}

	void enable_tpdo()
	{
		Log() << "[ucanopen] Enabling client TPDO messages... ";
		_is_tpdo_enabled = true;
		Log() << "done.\n";
	}

	void disable_tpdo()
	{
		Log() << "[ucanopen] Disabling client TPDO messages... ";
		_is_tpdo_enabled = false;
		Log() << "done.\n";
	}

	void enable_server_rpdo()
	{
		for (auto& server : _servers)
		{
			server->rpdo_service.enable();
		}
	}

	void disable_server_rpdo()
	{
		for (auto& server : _servers)
		{
			server->rpdo_service.disable();
		}
	}

	void enable_server_watch()
	{
		for (auto& server : _servers)
		{
			server->watch_service.enable();
		}
	}

	void disable_server_watch()
	{
		for (auto& server : _servers)
		{
			server->watch_service.disable();
		}
	}

	void set_server_watch_period(std::chrono::milliseconds period)
	{
		for (auto& server : _servers)
		{
			server->watch_service.set_period(period);
		}
	}

protected:
	void _calculate_recvid(std::shared_ptr<Server> server);
	bool _is_free(NodeId nodeId) const;
};

} // namespace ucanopen

