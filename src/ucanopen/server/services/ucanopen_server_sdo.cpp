#include "ucanopen_server_sdo.h"


namespace ucanopen {

ServerSdoService::ServerSdoService(impl::Server* server)
	: _server(server)
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
	auto entry_iter = _server->dictionary().entries.find(key);
	if (entry_iter == _server->dictionary().entries.end())
	{
		return 3;
	}

	SdoType sdo_type;
	if (entry_iter->second.data_type == ODEntryDataType::OD_EXEC)
	{
		sdo_type = SdoType::response_to_exec;
		Log() << "[ucanopen/exec] " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " executed.\n";
	}
	else
	{
		sdo_type = SdoType::response_to_read;
		if (entry_iter->second.category != _server->dictionary().config.watch_category)
		{
			Log() << "[ucanopen/read] " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
					<< " = " << sdo.data.to_string(entry_iter->second.data_type) << '\n';
		}
	}

	for (auto& subscriber : _subscriber_list)
	{
		subscriber->handle_sdo(sdo_type, entry_iter, sdo.data);
	}

	// server-specific TSDO handling
	_server->_handle_tsdo(sdo_type, entry_iter, sdo.data);
	
	return 0;
}


int ServerSdoService::_handle_write_expedited(const can_frame& frame)
{
	ExpeditedSdo sdo(frame.data);
	ODEntryKey key = {sdo.index, sdo.subindex};
	auto entry_iter = _server->dictionary().entries.find(key);
	if (entry_iter == _server->dictionary().entries.end())
	{
		return 3;
	}

	SdoType sdo_type;
	if (entry_iter->second.data_type == ODEntryDataType::OD_EXEC)
	{
		sdo_type = SdoType::response_to_exec;
		Log() << "[ucanopen/exec] " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " executed.\n";
	}
	else
	{
		sdo_type = SdoType::response_to_write;
		Log() << "[ucanopen/write] " << entry_iter->second.category << "::" << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " updated.\n";
	}

	// server-specific TSDO handling
	_server->_handle_tsdo(sdo_type, entry_iter, sdo.data);

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
	Log() << "[ucanopen] SDO transfer aborted: " << error_msg << " (error code: 0x" << std::hex << abort_sdo.error_code << std::dec << ")\n";

	return 0;
}

} // namespace ucanopen

