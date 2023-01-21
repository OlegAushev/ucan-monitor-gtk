#include "ucanopen_server_rpdo.h"


namespace ucanopen {

ServerRpdoService::ServerRpdoService(impl::Server* server)
	: _server(server)
{}


void ServerRpdoService::registerRpdo(RpdoType type, std::chrono::milliseconds period, std::function<can_payload(void)> creator)
{
	canid_t id = calculate_cob_id(to_cob_type(type), _server->node_id());
	_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now(), creator}});
}


void ServerRpdoService::update_node_id()
{
	for (auto& [type, message] : _rpdoList)
	{
		message.id = calculate_cob_id(to_cob_type(type), _server->node_id());
	}
}

} // namespace ucanopen

