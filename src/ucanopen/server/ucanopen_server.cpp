#include "ucanopen_server.h"


namespace ucanopen {

Server::Server(const std::string& name, NodeId node_id, std::shared_ptr<can::Socket> socket, const ObjectDictionary& dictionary)
	: impl::Server(name, node_id, socket, dictionary)
	, heartbeat_service(this, std::chrono::milliseconds(2000))
	, tpdo_service(this)
	, rpdo_service(this)
	, sdo_service(this)
	, watch_service(this, &sdo_service)
	, config_service(this)
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
		if (status == 0)
		{
			break;
		}
	}	
}


uint32_t Server::get_serial_number()
{
	std::promise<void> signal_terminate;
	utils::SerialNumberGetter sn_getter(this, &sdo_service);
	std::future<uint32_t> sn_future = std::async(&utils::SerialNumberGetter::get, &sn_getter, signal_terminate.get_future());
	if (sn_future.wait_for(std::chrono::milliseconds(1000)) != std::future_status::ready)
	{
		signal_terminate.set_value();
		return sn_future.get();
	}
	return sn_future.get();
}


std::string Server::get_device_name()
{
	std::promise<void> signal_terminate;
	utils::DeviceNameGetter device_name_getter(this, &sdo_service);
	std::future<std::string> sn_future = std::async(&utils::DeviceNameGetter::get, &device_name_getter, signal_terminate.get_future());
	if (sn_future.wait_for(std::chrono::milliseconds(1000)) != std::future_status::ready)
	{
		signal_terminate.set_value();
		return sn_future.get();
	}
	return sn_future.get();
}

} // namespace ucanopen

