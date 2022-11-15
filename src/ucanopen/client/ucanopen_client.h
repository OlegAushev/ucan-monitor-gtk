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
	std::shared_ptr<can::Socket> m_canSocket;
	NmtState m_state;

	std::vector<std::shared_ptr<IServer>> m_servers;
	std::map<canid_t, std::shared_ptr<IServer>> m_recvIdServerList;

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
		std::function<std::array<uint8_t, 8>(void)> creator;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoList;

	/* THREADS */
	std::thread m_threadRun;
	std::promise<void> m_signalExitRunThread;
	void run(std::future<void> futureExit);

	void onFrameReceived(can_frame frame);

public:
	Client(NodeId t_nodeId, std::shared_ptr<can::Socket> t_canSocket);
	~Client();

	void registerServer(std::shared_ptr<IServer> server);

	void setHeartbeatPeriod(std::chrono::milliseconds period)
	{
		m_heartbeatInfo.period = period;
	}
	
	void registerTpdo(TpdoType tpdoType, std::chrono::milliseconds period, std::function<std::array<uint8_t, 8>(void)> callback)
	{
		m_tpdoList[tpdoType].period = period;
		m_tpdoList[tpdoType].creator = callback;
	}

	void setNodeId(NodeId _nodeId)
	{
		nodeId = _nodeId;
	}

	void setTpdoEnabled(bool isEnabled) { m_isTpdoEnabled = isEnabled; }
	void sendTpdo();

};


} // namespace ucanopen


