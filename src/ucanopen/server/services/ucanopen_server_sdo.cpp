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


int ServerSdoService::handle_frame(const can_frame& frame)
{
	if (frame.can_id != _id) return 1;
	
	uint32_t cs_code = get_cs_code(frame);

	switch (cs_code)
	{
		case sdo_cs_codes::server_init_read:
			return _handle_read_expedited(frame);
		case sdo_cs_codes::server_init_write:
			return _handle_write_expedited(frame);	
		case sdo_cs_codes::abort:
			return _handle_abort(frame);
		default:
			return 2;
	}
}


int ServerSdoService::_handle_read_expedited(const can_frame& frame)
{
	ExpeditedSdo sdo(frame.data);
	ODEntryKey key = {sdo.index, sdo.subindex};
	auto od_entry = _server->_dictionary.find(key);
	if (od_entry == _server->_dictionary.end())
	{
		return 3;
	}

	SdoType sdo_type;
	if (od_entry->second.data_type == ODEntryDataType::OD_EXEC)
	{
		sdo_type = SdoType::response_to_exec;
	}
	else
	{
		sdo_type = SdoType::response_to_read;
	}

	// handle watch data
	_watch_service->handle_frame(sdo_type, od_entry, sdo.data);

	// server-specific TSDO handling
	_server->_handle_tsdo(sdo_type, od_entry, sdo.data);
	
	return 0;
}


int ServerSdoService::_handle_write_expedited(const can_frame& frame)
{
	ExpeditedSdo sdo(frame.data);
	ODEntryKey key = {sdo.index, sdo.subindex};
	auto od_entry = _server->_dictionary.find(key);
	if (od_entry == _server->_dictionary.end())
	{
		return 3;
	}

	SdoType sdo_type;
	if (od_entry->second.data_type == ODEntryDataType::OD_EXEC)
	{
		sdo_type = SdoType::response_to_exec;
	}
	else
	{
		sdo_type = SdoType::response_to_write;
	}

	// server-specific TSDO handling
	_server->_handle_tsdo(sdo_type, od_entry, sdo.data);

	return 0;
}


int ServerSdoService::_handle_abort(const can_frame& frame)
{
	AbortSdo abort_sdo(frame.data);
	std::string error_msg;
	if (sdo_abort_messages.contains(static_cast<SdoAbortCode>(abort_sdo.error_code)))
	{
		error_msg = sdo_abort_messages.at(static_cast<SdoAbortCode>(abort_sdo.error_code));
	}
	else
	{
		error_msg = "reason unknown";
	}
	Log() << "[ucanopen] SDO transfer aborted: " << error_msg << "(error code: 0x" << std::hex << abort_sdo.error_code << std::dec << ")\n";

	return 0;
}

} // namespace ucanopen

