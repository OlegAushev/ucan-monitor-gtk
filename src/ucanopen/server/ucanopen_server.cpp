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
{}


void Server::_set_node_id(NodeId node_id)
{
	if (!node_id.is_valid()) return;

	_node_id = node_id;

	heartbeat_service.update_node_id();
	tpdo_service.update_node_id();
	rpdo_service.update_node_id();
}


void Server::_send()
{	
	rpdo_service.send();
	watch_service.send();
}


void Server::_handle_frame(const can_frame& frame)
{
	if (tpdo_service.handle_frame(frame))
	{
		return;
	}
	else if (frame.can_id == calculate_cob_id(CobType::tsdo, _node_id))
	{
		CobSdo sdo_message(frame.data);
		ODEntryKey key = {sdo_message.index, sdo_message.subindex};
		auto od_entry = _dictionary.find(key);
		if (od_entry == _dictionary.end())
		{
			return;
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
				return;
		}
		
		// handle watch data
		watch_service.handle_frame(sdo_type, od_entry, sdo_message.data);

		// server-specific TSDO handling
		_handle_tsdo(sdo_type, od_entry, sdo_message.data);
	}
	else if (heartbeat_service.handle_frame(frame))
	{
		return;
	}	
}

} // namespace ucanopen

