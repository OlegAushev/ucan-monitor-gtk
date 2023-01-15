/**
 * @file ucanopen_server_heartbeat.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_server_heartbeat.h"


namespace ucanopen {


///
///
///
ServerHeartbeatService::ServerHeartbeatService(impl::Server* server, std::chrono::milliseconds timeout)
	: _server(server)
{
	_id = calculateCobId(CobType::Heartbeat, _server->nodeId());
	_timeout = timeout;
	_timepoint = std::chrono::steady_clock::now();
	_nmtState = NmtState::Stopped;
}


///
///
///
void ServerHeartbeatService::updateNodeId()
{
	_id = calculateCobId(CobType::Heartbeat, _server->nodeId());
}


}

