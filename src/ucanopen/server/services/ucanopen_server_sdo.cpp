#include "ucanopen_server_sdo.h"


namespace ucanopen {

ServerSdoService::ServerSdoService(impl::Server* server, ServerWatchService* watch_service)
	: _server(server)
	, _watch_service(watch_service)
{
	_id = calculate_cob_id(CobType::tsdo, _server->node_id());
}


void ServerSdoService::update_node_id()
{
	_id = calculate_cob_id(CobType::tsdo, _server->node_id());
}

} // namespace ucanopen

