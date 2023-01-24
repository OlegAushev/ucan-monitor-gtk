#include "ucanopen_server.h"


namespace ucanopen {

Server::Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionary_config)
	: impl::Server(name, node_id, socket, dictionary)
	, heartbeat_service(this, std::chrono::milliseconds(2000))
	, tpdo_service(this)
	, rpdo_service(this)
	, watch_service(this, dictionary, dictionary_config)
	, config_service(this, dictionary, dictionary_config)
	, sdo_service(this, &watch_service)
{
	_frame_handling_services.push_back(&sdo_service);
	_frame_handling_services.push_back(&tpdo_service);
	_frame_handling_services.push_back(&heartbeat_service);
}


void Server::_set_node_id(NodeId node_id)
{
	if (!node_id.is_valid()) return;

	_node_id = node_id;

	heartbeat_service.update_node_id();
	tpdo_service.update_node_id();
	rpdo_service.update_node_id();
	sdo_service.update_node_id();
}


void Server::_send()
{	
	rpdo_service.send();
	watch_service.send();
}


void Server::_handle_frame(const can_frame& frame)
{
	for (auto service : _frame_handling_services)
	{
		auto status = service->handle_frame(frame);
		if (status == HandlingStatus::success)
		{
			break;
		}
	}	
}

} // namespace ucanopen

