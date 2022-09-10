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
Client::Client(NodeId _nodeId, std::shared_ptr<can::Socket> _canSocket)
	: nodeId(_nodeId)
	, m_canSocket(_canSocket)
	, m_state(NmtState::INITIALIZATION)
{
	m_heartbeatInfo.period = std::chrono::milliseconds(1000);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	m_isTpdoEnabled = true;

	m_tpdoInfo.insert({TpdoType::TPDO1, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO2, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO3, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO4, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});

#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Starting aux thread..." << std::endl;
#endif
	std::future<void> futureExit = m_signalExitRunThread.get_future();
	m_threadRun = std::thread(&Client::run, this, std::move(futureExit));
	m_state = NmtState::OPERATIONAL;
}


///
///
///
Client::~Client()
{
#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Sending signal to aux thread to stop..." << std::endl;
#endif
	m_signalExitRunThread.set_value();
	m_threadRun.join();
}


///
///
///
void Client::run(std::future<void> futureExit)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Aux thread has started." << std::endl;
#endif

	while (futureExit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* HEARTBEAT */
		if (now - m_heartbeatInfo.timepoint > m_heartbeatInfo.period)
		{
			m_canSocket->send(makeFrame(CobType::HEARTBEAT, nodeId, {static_cast<uint8_t>(m_state)}));
			m_heartbeatInfo.timepoint = now;
		}

		/* TPDO */
		sendTpdo();
		for (auto& serverNode : serverNodes)
		{
			serverNode.second.sendRpdo();
		}

		/* RECV */
		can_frame recvFrame;
		can::Error recvErr = m_canSocket->recv(recvFrame);
		while (recvErr == can::Error::NO_ERROR)
		{
			(void) std::async(&Client::onFrameReceived, this, recvFrame);
			recvErr = m_canSocket->recv(recvFrame);
		}
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Aux thread has stopped." << std::endl;
#endif
}


///
///
///
void Client::sendTpdo()
{
	auto now = std::chrono::steady_clock::now();
	if (m_isTpdoEnabled)
	{
		for (auto& tpdo : m_tpdoInfo)
		{
			if (tpdo.second.callbackOnSend)
			{
				if (now - tpdo.second.timepoint >= tpdo.second.period)
				{
					m_canSocket->send(makeFrame(toCobType(tpdo.first), nodeId, tpdo.second.callbackOnSend()));
					tpdo.second.timepoint = now;
				}
			}
		}
	}
}


///
///
///
void Client::onFrameReceived(can_frame frame)
{
	for (auto& serverNode : serverNodes)
	{
		serverNode.second.onFrameReceived(frame);
	}
}


} // namespace ucanopen


