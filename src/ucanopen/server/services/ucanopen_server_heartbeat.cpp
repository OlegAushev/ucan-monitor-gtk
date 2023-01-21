#include "ucanopen_server_heartbeat.h"


namespace ucanopen {

ServerHeartbeatService::ServerHeartbeatService(impl::Server* server, std::chrono::milliseconds timeout)
	: _server(server)
{
	_id = calculate_cob_id(CobType::heartbeat, _server->node_id());
	_timeout = timeout;
	_timepoint = std::chrono::steady_clock::now();
}


void ServerHeartbeatService::update_node_id()
{
	_id = calculate_cob_id(CobType::heartbeat, _server->node_id());
}

} // namespace ucanopen

