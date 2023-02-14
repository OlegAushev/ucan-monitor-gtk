#pragma once


#include "impl/ucanopen_impl_server.h"
#include "services/ucanopen_server_heartbeat.h"
#include "services/ucanopen_server_tpdo.h"
#include "services/ucanopen_server_rpdo.h"
#include "services/ucanopen_server_watch.h"
#include "services/ucanopen_server_config.h"
#include "services/ucanopen_server_sdo.h"
#include <atomic>
#include <functional>
#include <chrono>
#include <future>


namespace ucanopen {

class Server : public impl::Server
{
	friend class Client;
public:
	ServerHeartbeatService heartbeat_service;
	ServerTpdoService tpdo_service;
	ServerRpdoService rpdo_service;
	ServerWatchService watch_service;
	ServerConfigService config_service;
	ServerSdoService sdo_service;
public:
	Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket, const ObjectDictionary& dictionary);
	virtual ~Server() = default;
private:	
	void _send();
	void _handle_frame(const can_frame& frame);
	void _set_node_id(NodeId nodeId);

	std::vector<impl::FrameHandlingService*> _frame_handling_services;
};

} // namespace ucanopen

