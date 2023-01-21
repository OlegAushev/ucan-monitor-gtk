#include "ucanopen_server_tpdo.h"


namespace ucanopen {

ServerTpdoService::ServerTpdoService(impl::Server* server)
	: _server(server)
{}


void ServerTpdoService::register_tpdo(TpdoType tpdo_type, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler)
{
	canid_t id = calculate_cob_id(to_cob_type(tpdo_type), _server->node_id());
	_tpdo_list.insert({tpdo_type, {id, timeout, std::chrono::steady_clock::now(), can_payload{}, handler}});
}


void ServerTpdoService::update_node_id()
{
	for (auto& [tpdo_type, message] : _tpdo_list)
	{
		message.id = calculate_cob_id(to_cob_type(tpdo_type), _server->node_id());
	}
}

} // namespace ucanopen

