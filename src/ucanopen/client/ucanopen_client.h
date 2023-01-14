/**
 * @file ucanopen_client.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <functional>
#include <thread>
#include <future>
#include <atomic>
#include <chrono>
#include <map>

#include <sstream>
#include <iomanip>

#include "cansocket/cansocket.h"
#include "../ucanopen_def.h"
#include "../server/ucanopen_server.h"
#include "../tester/ucanopen_tester.h"


namespace ucanopen {


class Client
{
private:
	NodeId _nodeId;
	std::shared_ptr<can::Socket> _socket;
	NmtState _state;

	std::set<std::shared_ptr<Server>> _servers;
	std::map<canid_t, std::shared_ptr<Server>> _recvIdServerList;

	/* SYNC */
	struct SyncInfo
	{
		bool isEnabled = false;
		std::chrono::milliseconds period = std::chrono::milliseconds(1000);
		std::chrono::time_point<std::chrono::steady_clock> timepoint;		
	};
	SyncInfo _syncInfo;

	/* HEARTBEAT */
	struct HeartbeatInfo
	{
		std::chrono::milliseconds period = std::chrono::milliseconds(1000);
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	HeartbeatInfo _heartbeatInfo;
			
	/* TPDO client --> server */
	bool _isTpdoEnabled = false;
	struct TpdoInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload(void)> creator;
	};
	std::map<TpdoType, TpdoInfo> _tpdoList;

	/* THREADS */
	std::thread _threadRun;
	std::promise<void> _signalExitRunThread;
	void _run(std::future<void> futureExit);

	void _onFrameReceived(const can_frame& frame);

public:
	Client(NodeId nodeId_, std::shared_ptr<can::Socket> socket);
	~Client();
	NodeId nodeId() const { return _nodeId;	}
	void setNodeId(NodeId nodeId);
	void registerServer(std::shared_ptr<Server> server);

	std::shared_ptr<Server> server(std::string_view name)
	{
		auto itServer = std::find_if(_servers.begin(), _servers.end(),
			[name](const auto& s)
			{
				return s->name() == name;
			});
		if (itServer == _servers.end())
		{
			return nullptr;
		}
		return *itServer;
	}

	void setServerNodeId(std::string_view name, NodeId nodeId);

	void enableSync()
	{
		std::cout << "[ucanopen] Enabling client SYNC messages (period = " << _syncInfo.period << ")... ";	
		_syncInfo.isEnabled = true;
		std::cout << "done." << std::endl;
	}

	void disableSync()
	{
		std::cout << "[ucanopen] Disabling client SYNC messages... ";
		_syncInfo.isEnabled = false;
		std::cout << "done." << std::endl;
	}

	void setSyncPeriod(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting client SYNC messages period = " << period << "... ";
		_syncInfo.period = period;
		std::cout << "done." << std::endl;
	}

	void setHeartbeatPeriod(std::chrono::milliseconds period)
	{
		_heartbeatInfo.period = period;
	}

	void registerTpdo(TpdoType tpdoType, std::chrono::milliseconds period, std::function<can_payload(void)> creator)
	{
		_tpdoList.insert({tpdoType, {period, std::chrono::steady_clock::now(), creator}});
	}

	void enableTpdo()
	{
		std::cout << "[ucanopen] Enabling client TPDO messages... ";
		_isTpdoEnabled = true;
		std::cout << "done." << std::endl;
	}

	void disableTpdo()
	{
		std::cout << "[ucanopen] Disabling client TPDO messages... ";
		_isTpdoEnabled = false;
		std::cout << "done." << std::endl;
	}

	void enableServerWatch()
	{
		for (auto& server : _servers)
		{
			server->watchService.enable();
		}
	}

	void disableServerWatch()
	{
		for (auto& server : _servers)
		{
			server->watchService.disable();
		}
	}

	void setServerWatchPeriod(std::chrono::milliseconds period)
	{
		for (auto& server : _servers)
		{
			server->watchService.setPeriod(period);
		}
	}

protected:
	void _calculateRecvId(std::shared_ptr<Server> server);
	bool _isFree(NodeId nodeId) const;
};


} // namespace ucanopen


