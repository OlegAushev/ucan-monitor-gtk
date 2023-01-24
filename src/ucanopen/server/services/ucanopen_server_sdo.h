#pragma once


#include "../impl/ucanopen_impl_server.h"
#include "ucanopen_server_watch.h"


namespace ucanopen {

class ServerSdoService : public impl::FrameHandlingService
{
private:
	impl::Server* const _server;
	ServerWatchService* const _watch_service;
	canid_t _id;
public:
	ServerSdoService(impl::Server* server, ServerWatchService* watch_service);
	void update_node_id();

	virtual HandlingStatus handle_frame(const can_frame& frame)
	{
		if (frame.can_id != _id) return HandlingStatus::invalid_id;
		
		CobSdo sdo_message(frame.data);
		ODEntryKey key = {sdo_message.index, sdo_message.subindex};
		auto od_entry = _server->_dictionary.find(key);
		if (od_entry == _server->_dictionary.end())
		{
			return HandlingStatus::fail;
		}

		SdoType sdo_type;
		switch (sdo_message.cs)
		{
			case cs_codes::sdo_scs_read:
				if (od_entry->second.data_type == ODEntryDataType::OD_EXEC)
				{
					sdo_type = SdoType::response_to_exec;
				}
				else
				{
					sdo_type = SdoType::response_to_read;
				}
				break;
			case cs_codes::sdo_scs_write:
				sdo_type = SdoType::response_to_write;
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
};

} // namespace ucanopen

