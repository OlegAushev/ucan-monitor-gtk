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


int ServerHeartbeatService::handle_frame(const can_frame& frame)
{
	if (frame.can_id != _id) return 1;
	
	_timepoint = std::chrono::steady_clock::now();
	_server->_nmt_state = static_cast<NmtState>(frame.data[0]);
	return 0;
}

} // namespace ucanopen

