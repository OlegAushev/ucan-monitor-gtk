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
Client::Client(NodeId nodeId_, std::shared_ptr<can::Socket> socket)
	: nodeId(nodeId_)
	, m_socket(socket)
	, m_state(NmtState::INITIALIZATION)
{
	m_syncInfo.period = std::chrono::milliseconds(0);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	m_heartbeatInfo.period = std::chrono::milliseconds(1000);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Starting aux thread..." << std::endl;
#endif
	std::future<void> futureExit = m_signalExitRunThread.get_future();
	m_threadRun = std::thread(&Client::run, this, std::move(futureExit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
	m_servers.insert(server);

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

		/* SYNC */
		if (m_syncInfo.period != std::chrono::milliseconds(0))
		{
			if (now - m_syncInfo.timepoint > m_syncInfo.period)
			{
				m_socket->send(makeFrame(CobType::SYNC, nodeId, {}));
				m_syncInfo.timepoint = now;
			}
		}

		/* HEARTBEAT */
		if (now - m_heartbeatInfo.timepoint > m_heartbeatInfo.period)
		{
			m_socket->send(makeFrame(CobType::HEARTBEAT, nodeId, {static_cast<uint8_t>(m_state)}));
			m_heartbeatInfo.timepoint = now;
		}

		/* TPDO */
		if (m_isTpdoEnabled)
		{
			for (auto& [type, message] : m_tpdoList)
			{
				if (!message.creator) continue;
				if (now - message.timepoint >= message.period)
				{
					m_socket->send(makeFrame(toCobType(type), nodeId, message.creator()));
					message.timepoint = now;
				}
			}
		}

		/* SERVER's RPDO */
		for (auto& server : m_servers)
		{
			server->checkConnection();
			server->sendRpdo();
		}

		/* RECV */
		can_frame frame;
		can::Error recvErr = m_socket->recv(frame);
		while (recvErr == can::Error::NoError)
		{
			(void) std::async(&Client::onFrameReceived, this, frame);
			recvErr = m_socket->recv(frame);
		}
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[ucanopen] Aux thread has stopped." << std::endl;
#endif
}


///
///
///
void Client::onFrameReceived(const can_frame& frame)
{
	auto it = m_recvIdServerList.find(frame.can_id);
	if (it != m_recvIdServerList.end())
	{
		it->second->handleFrame(frame);
	}
}


} // namespace ucanopen


