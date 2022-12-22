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

	std::set<std::shared_ptr<IServer>> _servers;
	std::map<canid_t, std::shared_ptr<IServer>> _recvIdServerList;

	/* SYNC */
	struct SyncInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;		
	};
	SyncInfo _syncInfo;


	/* HEARTBEAT */
	struct HeartbeatInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	HeartbeatInfo _heartbeatInfo;
			
	/* TPDO client --> server */
	bool _isTpdoEnabled{false};
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
	void run(std::future<void> futureExit);

	void onFrameReceived(const can_frame& frame);

public:
	/**
	 * @brief Construct a new Client object
	 * 
	 * @param nodeId_ 
	 * @param socket 
	 */
	Client(NodeId nodeId_, std::shared_ptr<can::Socket> socket);

	/**
	 * @brief Destroy the Client object
	 * 
	 */
	~Client();

	/**
	 * @brief Returns client node ID.
	 * 
	 * @return NodeId 
	 */
	NodeId nodeId() const
	{
		return _nodeId;
	}

	/**
	 * @brief Sets client node ID.
	 * 
	 * @param nodeId_ 
	 */
	void setNodeId(NodeId nodeId);
	
	/**
	 * @brief 
	 * 
	 * @param server 
	 */
	void registerServer(std::shared_ptr<IServer> server);

	/**
	 * @brief Returns pointer to server
	 * 
	 * @param name 
	 * @return std::shared_ptr<IServer> 
	 */
	std::shared_ptr<IServer> server(std::string_view name)
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

	/**
	 * @brief Set the Server Node Id object
	 * 
	 * @param name 
	 * @param nodeId 
	 */
	void setServerNodeId(std::string_view name, NodeId nodeId);

	/* SYNC */
	/**
	 * @brief 
	 * 
	 * @param period 
	 */
	void enableSync(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Enabling client SYNC messages (period = " << period << ")... ";	
		_syncInfo.period = period;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disableSync()
	{
		std::cout << "[ucanopen] Disabling client SYNC messages... ";	
		_syncInfo.period = std::chrono::milliseconds(0);
		std::cout << "done." << std::endl;
	}

	/* HEARTBEAT */
	/**
	 * @brief Set the Heartbeat Period object
	 * 
	 * @param period 
	 */
	void setHeartbeatPeriod(std::chrono::milliseconds period)
	{
		_heartbeatInfo.period = period;
	}

	/* TPDO */
	/**
	 * @brief 
	 * 
	 * @param tpdoType 
	 * @param period 
	 * @param creator 
	 */
	void registerTpdo(TpdoType tpdoType, std::chrono::milliseconds period, std::function<can_payload(void)> creator)
	{
		_tpdoList.insert({tpdoType, {period, std::chrono::steady_clock::now(), creator}});
	}

	/**
	 * @brief 
	 * 
	 */
	void enableTpdo()
	{
		std::cout << "[ucanopen] Enabling client TPDO messages... ";
		_isTpdoEnabled = true;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disableTpdo()
	{
		std::cout << "[ucanopen] Disabling client TPDO messages... ";
		_isTpdoEnabled = false;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void enableServerWatch()
	{
		for (auto& server : _servers)
		{
			server->enableWatch();
		}
	}

	/**
	 * @brief 
	 * 
	 */
	void disableServerWatch()
	{
		for (auto& server : _servers)
		{
			server->disableWatch();
		}
	}

	/**
	 * @brief 
	 * 
	 */
	void setServerWatchPeriod(std::chrono::milliseconds period)
	{
		for (auto& server : _servers)
		{
			server->setWatchPeriod(period);
		}
	}

protected:
	void calculateRecvId(std::shared_ptr<IServer> server);
	bool isFree(NodeId nodeId) const;
};


} // namespace ucanopen


