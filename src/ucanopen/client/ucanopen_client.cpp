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
Client::Client(NodeId t_nodeId, std::shared_ptr<can::Socket> t_canSocket)
	: nodeId(t_nodeId)
	, m_canSocket(t_canSocket)
	, m_state(NmtState::INITIALIZATION)
{
	m_heartbeatInfo.period = std::chrono::milliseconds(1000);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	m_tpdoList.insert({TpdoType::TPDO1, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoList.insert({TpdoType::TPDO2, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoList.insert({TpdoType::TPDO3, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_tpdoList.insert({TpdoType::TPDO4, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});

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
void Client::registerServer(std::shared_ptr<IServer> server)
{
	m_servers.push_back(server);

	canid_t tpdo1 = calculateCobId(CobType::TPDO1, server->nodeId.value());
	canid_t tpdo2 = calculateCobId(CobType::TPDO2, server->nodeId.value());
	canid_t tpdo3 = calculateCobId(CobType::TPDO3, server->nodeId.value());
	canid_t tpdo4 = calculateCobId(CobType::TPDO4, server->nodeId.value());
	canid_t tsdo = calculateCobId(CobType::TSDO, server->nodeId.value());

	m_recvIdServerList.insert({tpdo1, server});
	m_recvIdServerList.insert({tpdo2, server});
	m_recvIdServerList.insert({tpdo3, server});
	m_recvIdServerList.insert({tpdo4, server});
	m_recvIdServerList.insert({tsdo, server});
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
		for (auto& server : m_servers)
		{
			server->checkConnection();
			server->sendRpdo();
		}

		/* RECV */
		can_frame frame;
		can::Error recvErr = m_canSocket->recv(frame);
		while (recvErr == can::Error::NO_ERROR)
		{
			(void) std::async(&Client::onFrameReceived, this, frame);
			recvErr = m_canSocket->recv(frame);
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
		for (auto& tpdo : m_tpdoList)
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
	auto it = m_recvIdServerList.find(frame.can_id);
	if (it != m_recvIdServerList.end())
	{
		it->second->processFrame(frame);
	}
}


} // namespace ucanopen


