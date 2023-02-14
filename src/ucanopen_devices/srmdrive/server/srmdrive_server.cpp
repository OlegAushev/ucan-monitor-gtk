#include "srmdrive_server.h"


namespace srmdrive {

Server::Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, node_id, socket, object_dictionary)
	, controller(this)
{
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo1(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(1200),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo2(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo3(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo4(payload); });
}


void Server::_handle_tsdo(ucanopen::SdoType sdo_type,
			ucanopen::ObjectDictionaryEntries::const_iterator entry_iter,
			ucanopen::ExpeditedSdoData sdo_data)
{
	if (entry_iter->second.category == _dictionary.config.watch_category && entry_iter->second.data_type == ucanopen::OD_ENUM16)
	{
		if (entry_iter->second.name == "DRIVE_STATE" && sdo_data.u32() < drive_states.size())
		{
			watch_service.set_value(entry_iter->second.name, drive_states[sdo_data.u32()]);
		}
	}
}


void Server::_handle_tpdo3(const ucanopen::can_payload& payload)
{
	CobTpdo3 message = ucanopen::from_payload<CobTpdo3>(payload);
	if ((message.syslog_message_id != 0) && (message.syslog_message_id < syslog_messages.size()))
	{
		Log() << syslog_messages[message.syslog_message_id] << '\n';
	}
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload)
{
	CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(payload);
	_errors = message.errors;
	_warnings = message.warnings;
}

}

