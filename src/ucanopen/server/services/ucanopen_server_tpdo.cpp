/**
 * @file ucanopen_server_tpdo.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_server_tpdo.h"


namespace ucanopen {


///
///
///
ServerTpdoService::ServerTpdoService(impl::Server* server)
	: _server(server)
{}


///
///
///
void ServerTpdoService::registerTpdo(TpdoType type, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler)
{
	canid_t id = calculate_cob_id(to_cob_type(type), _server->nodeId());
	_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now(), can_payload{}, handler}});
}


///
///
///
void ServerTpdoService::updateNodeId()
{
	for (auto& [type, message] : _tpdoList)
	{
		message.id = calculate_cob_id(to_cob_type(type), _server->nodeId());
	}
}


}


