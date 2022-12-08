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
public:
	NodeId nodeId;
private:
	std::shared_ptr<can::Socket> m_socket;
	NmtState m_state;

	std::set<std::shared_ptr<IServer>> m_servers;
	std::map<canid_t, std::shared_ptr<IServer>> m_recvIdServerList;

	/* SYNC */
	struct SyncInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;		
	};
	SyncInfo m_syncInfo;


	/* HEARTBEAT */
	struct HeartbeatInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	HeartbeatInfo m_heartbeatInfo;
			
	/* TPDO client --> server */
	bool m_isTpdoEnabled{false};
	struct TpdoInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload(void)> creator;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoList;

	/* THREADS */
	std::thread m_threadRun;
	std::promise<void> m_signalExitRunThread;
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
	 * @brief Set the Node Id object
	 * 
	 * @param _nodeId 
	 */
	void setNodeId(NodeId _nodeId)
	{
		nodeId = _nodeId;
	}
	
	/**
	 * @brief 
	 * 
	 * @param server 
	 */
	void registerServer(std::shared_ptr<IServer> server);

	/* SYNC */
	/**
	 * @brief 
	 * 
	 * @param period 
	 */
	void enableSync(std::chrono::milliseconds period)
	{
		m_syncInfo.period = period;
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Client SYNC message has been enabled, period = "
				<< period << "." << std::endl;
#endif		
	}

	/**
	 * @brief 
	 * 
	 */
	void disableSync()
	{
		m_syncInfo.period = std::chrono::milliseconds(0);
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Client SYNC message has been disabled." << std::endl;
#endif		
	}

	/* HEARTBEAT */
	/**
	 * @brief Set the Heartbeat Period object
	 * 
	 * @param period 
	 */
	void setHeartbeatPeriod(std::chrono::milliseconds period)
	{
		m_heartbeatInfo.period = period;
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
		m_tpdoList.insert({tpdoType, {period, std::chrono::steady_clock::now(), creator}});
	}

	/**
	 * @brief 
	 * 
	 */
	void enableTpdo()
	{
		m_isTpdoEnabled = true;
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Client TPDO messages have been enabled." << std::endl;
#endif
	}

	/**
	 * @brief 
	 * 
	 */
	void disableTpdo()
	{
		m_isTpdoEnabled = false;
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Client TPDO messages have been disabled." << std::endl;
#endif
	}

	/**
	 * @brief 
	 * 
	 */
	void enableServerWatch()
	{
		for (auto& server : m_servers)
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
		for (auto& server : m_servers)
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
		for (auto& server : m_servers)
		{
			server->setWatchPeriod(period);
		}
	}
};


} // namespace ucanopen


