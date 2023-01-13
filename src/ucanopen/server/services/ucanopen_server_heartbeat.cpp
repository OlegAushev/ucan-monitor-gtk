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


#include "ucanopen_service_heartbeat.h"


namespace ucanopen {


///
///
///
ServerHeartbeatService::ServerHeartbeatService(NodeId nodeId, std::chrono::milliseconds timeout)
{
	_id = calculateCobId(CobType::Heartbeat, nodeId);
	_timeout = timeout;
	_timepoint = std::chrono::steady_clock::now();
	_nmtState = NmtState::Stopped;
}


///
///
///
void ServerHeartbeatService::setNodeId(NodeId nodeId)
{
	_id = calculateCobId(CobType::Heartbeat, nodeId);
}


}


