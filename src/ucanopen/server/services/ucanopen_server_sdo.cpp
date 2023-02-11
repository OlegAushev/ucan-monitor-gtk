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


HandlingStatus ServerSdoService::handle_frame(const can_frame& frame)
{
	if (frame.can_id != _id) return HandlingStatus::invalid_id;
	
	ExpeditedSdo sdo_message(frame.data);
	ODEntryKey key = {sdo_message.index, sdo_message.subindex};
	auto od_entry = _server->_dictionary.find(key);
	if (od_entry == _server->_dictionary.end())
	{
		return HandlingStatus::fail;
	}

	SdoType sdo_type;
	switch (sdo_message.cs)
	{
		case sdo_cs_codes::server_init_read:
			if (od_entry->second.data_type == ODEntryDataType::OD_EXEC)
			{
				sdo_type = SdoType::response_to_exec;
			}
			else
			{
				sdo_type = SdoType::response_to_read;
			}
			break;
		case sdo_cs_codes::server_init_write:
			if (od_entry->second.data_type == ODEntryDataType::OD_EXEC)
			{
				sdo_type = SdoType::response_to_exec;
			}
			else
			{
				sdo_type = SdoType::response_to_write;
			}	
			break;
		case sdo_cs_codes::abort:
			sdo_type = SdoType::abort;
			//std::stringstream abort_message;
			//abort_message << "[ucanopen] SDO transfer aborted: " << 
			break;
		default:
			return HandlingStatus::fail;
	}
	
	// handle watch data
	_watch_service->handle_frame(sdo_type, od_entry, sdo_message.data);

	// server-specific TSDO handling
	_server->_handle_tsdo(sdo_type, od_entry, sdo_message.data);
	
	return HandlingStatus::success;
}

} // namespace ucanopen

