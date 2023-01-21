/**
 * @file ucanopen_server_rpdo.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_server_rpdo.h"


namespace ucanopen {


///
///
///
ServerRpdoService::ServerRpdoService(impl::Server* server)
	: _server(server)
{}


///
///
///
void ServerRpdoService::registerRpdo(RpdoType type, std::chrono::milliseconds period, std::function<can_payload(void)> creator)
{
	canid_t id = calculate_cob_id(to_cob_type(type), _server->nodeId());
	_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now(), creator}});
}


///
///
///
void ServerRpdoService::updateNodeId()
{
	for (auto& [type, message] : _rpdoList)
	{
		message.id = calculate_cob_id(to_cob_type(type), _server->nodeId());
	}
}


}


