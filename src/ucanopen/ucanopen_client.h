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
#include "ucanopen_def.h"
#include "ucanopen_tpdodef.h"
#include "ucanopen_rpdodef.h"
#include "ucanopen_servernode/ucanopen_servernode.h"
#include "ucanopen_tester/ucanopen_tester.h"
#include "objectdictionary/objectdictionary.h"


namespace ucanopen {


class Client
{
public:
	NodeId nodeId;
private:
	std::shared_ptr<can::Socket> m_canSocket;
	NmtState m_state;
public:
	std::map<ServerNode::Name, ServerNode> serverNodes;

private:
	/* HEARTBEAT */
	struct HeartbeatInfo
	{
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	HeartbeatInfo m_heartbeatInfo;
			
	/* TPDO client --> server */
	bool m_isTpdoEnabled;
	struct TpdoInfo
	{
		std::function<std::array<uint8_t, 8>(void)> callbackOnSend;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoInfo;

	/* THREADS */
	std::thread m_threadRun;
	std::promise<void> m_signalExitRunThread;

public:
	Client(NodeId _nodeId, std::shared_ptr<can::Socket> _canSocket);
	~Client();
	void run(std::future<void> futureExit);

private:
	void onFrameReceived(can_frame frame);

public:
	void setHeartbeatPeriod(std::chrono::milliseconds period)
	{
		m_heartbeatInfo.period = period;
	}
	
	void registerCallbackOnSendPdo(TpdoType tpdoType, std::function<std::array<uint8_t, 8>(void)> callback, std::chrono::milliseconds period)
	{
		m_tpdoInfo[tpdoType].callbackOnSend = callback;
		m_tpdoInfo[tpdoType].period = period;
	}

public:
	void enableTpdo() { m_isTpdoEnabled = true; }
	void disableTpdo() { m_isTpdoEnabled = false; }
	void sendTpdo();

	ODRequestStatus sendOdReadRequest(const std::string& category, const std::string& subcategory, const std::string& name);
	void sendOdWriteRequest(const std::string& category, const std::string& subcategory, const std::string& name, CobSdoData data = {});
	void sendOdWriteRequest(const std::string& category, const std::string& subcategory, const std::string& name, const std::string& value);
	void sendOdTaskRequest(const std::string& category, const std::string& subcategory, const std::string& name);
};


} // namespace ucanopen


