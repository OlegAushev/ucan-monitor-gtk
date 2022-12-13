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
	: m_nodeId(nodeId)
	, m_socket(socket)
	, m_state(NmtState::Initialization)
{
	m_syncInfo.period = std::chrono::milliseconds(0);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	m_heartbeatInfo.period = std::chrono::milliseconds(1000);
	m_heartbeatInfo.timepoint = std::chrono::steady_clock::now();

	std::cout << "[ucanopen] Starting aux thread..." << std::endl;

	std::future<void> futureExit = m_signalExitRunThread.get_future();
	m_threadRun = std::thread(&Client::run, this, std::move(futureExit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	m_state = NmtState::Operational;
}


///
///
///
Client::~Client()
{
	std::cout << "[ucanopen] Sending signal to aux thread to stop..." << std::endl;
	m_signalExitRunThread.set_value();
	m_threadRun.join();
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

	if (!isFree(nodeId))
	{
		std::cout << "failed: already occupied ID." << std::endl;
		return;
	}
	
	m_nodeId = nodeId;
	std::cout << "done." << std::endl;
}


///
///
///
void Client::registerServer(std::shared_ptr<IServer> server)
{
	std::cout << "[ucanopen] Adding '" << server->name() << "' server ID 0x" 
			<< std::hex << server->nodeId().value() << std::dec << " to client... ";

	auto itServerSameName = std::find_if(m_servers.begin(), m_servers.end(), 
		[server](const auto& s)
		{
			return server->name() == s->name();				
		});
	if (itServerSameName != m_servers.end())
	{
		std::cout << "failed: server with that name already added to client." << std::endl;
		return;
	}

	auto itServerSameId = std::find_if(m_servers.begin(), m_servers.end(), 
		[server](const auto& s)
		{
			return server->nodeId() == s->nodeId();				
		});
	if (itServerSameId != m_servers.end())
	{
		std::cout << "failed: server with ID 0x" << std::hex << server->nodeId().value() << std::dec
				<< " already added to client."  << std::endl;
		return;
	}

	if (server->nodeId() == m_nodeId)
	{
		std::cout << "failed: client has the same ID 0x" << std::hex << server->nodeId().value() << std::dec << std::endl;
		return;
	}

	m_servers.insert(server);
	calculateRecvId(server);

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

	if (!isFree(nodeId))
	{
		std::cout << "failed: already occupied ID." << std::endl;
		return;
	}

	auto itServer = std::find_if(m_servers.begin(), m_servers.end(),
		[name](const auto& s)
		{
			return s->name() == name;
		});
	if (itServer == m_servers.end())
	{
		std::cout << "failed: no such server found." << std::endl;
		return;
	}

	(*itServer)->setNodeId(nodeId);

	// erase outdated elements from [id; server] map
	for (auto it = m_recvIdServerList.begin(); it != m_recvIdServerList.end();)
	{
		if (it->second->name() == name)
		{
			it = m_recvIdServerList.erase(it);
		}
		else
		{
			++it;
		}
	}

	calculateRecvId(*itServer);

	std::cout << "done." << std::endl;
}


///
///
///
void Client::run(std::future<void> futureExit)
{
	std::cout << "[ucanopen] Aux thread started. Thread id: " << std::this_thread::get_id() << std::endl;

	while (futureExit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* SYNC */
		if (m_syncInfo.period != std::chrono::milliseconds(0))
		{
			if (now - m_syncInfo.timepoint > m_syncInfo.period)
			{
				m_socket->send(createFrame(CobType::Sync, m_nodeId, {}));
				m_syncInfo.timepoint = now;
			}
		}

		/* HEARTBEAT */
		if (now - m_heartbeatInfo.timepoint > m_heartbeatInfo.period)
		{
			m_socket->send(createFrame(CobType::Heartbeat, m_nodeId, {static_cast<uint8_t>(m_state)}));
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
					m_socket->send(createFrame(toCobType(type), m_nodeId, message.creator()));
					message.timepoint = now;
				}
			}
		}

		/* SERVER's RPDO */
		for (auto& server : m_servers)
		{
			server->checkConnection();
			server->sendPeriodic();
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

	std::cout << "[ucanopen] Aux thread stopped." << std::endl;
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


///
///
///
void Client::calculateRecvId(std::shared_ptr<IServer> server)
{
	canid_t tpdo1 = calculateCobId(CobType::Tpdo1, server->nodeId());
	canid_t tpdo2 = calculateCobId(CobType::Tpdo2, server->nodeId());
	canid_t tpdo3 = calculateCobId(CobType::Tpdo3, server->nodeId());
	canid_t tpdo4 = calculateCobId(CobType::Tpdo4, server->nodeId());
	canid_t tsdo = calculateCobId(CobType::Tsdo, server->nodeId());

	m_recvIdServerList.insert({tpdo1, server});
	m_recvIdServerList.insert({tpdo2, server});
	m_recvIdServerList.insert({tpdo3, server});
	m_recvIdServerList.insert({tpdo4, server});
	m_recvIdServerList.insert({tsdo, server});
}


///
///
///
bool Client::isFree(NodeId nodeId) const
{
	if (nodeId == m_nodeId)
	{
		return false;
	}

	for (const auto& server : m_servers)
	{
		if (nodeId == server->nodeId())
		{
			return false;
		}
	}

	return true;
}


} // namespace ucanopen


