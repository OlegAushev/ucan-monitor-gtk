#include "srmdrive_server.h"


namespace srmdrive {

Server::Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, node_id, socket, object_dictionary, object_dictionary_config)
	, controller(this)
{
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handle_tpdo1(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(1200),
			[this](ucanopen::can_payload data) { this->_handle_tpdo2(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handle_tpdo3(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handle_tpdo4(data); });
}


void Server::_handle_tsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entry_iter,
			ucanopen::CobSdoData sdo_data)
{
	if (entry_iter->second.category == watch_service.watch_category && entry_iter->second.data_type == ucanopen::OD_ENUM16)
	{
		if (entry_iter->second.name == "DRIVE_STATE" && sdo_data.u32() < drive_states.size())
		{
			watch_service.set_value(entry_iter->second.name, drive_states[sdo_data.u32()]);
		}
	}
	else if (entry_iter->second.category == config_service.config_category && sdoType == ucanopen::SdoType::response_to_read)
	{
		std::stringstream sstr;
		sstr << "[" << entry_iter->second.category << "/read] " << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " = " << sdo_data.to_string(entry_iter->second.data_type);
		Logger::instance().add(sstr.str());
	}
	else if (entry_iter->second.category == config_service.config_category && sdoType == ucanopen::SdoType::response_to_write)
	{
		std::stringstream sstr;
		sstr << "[" << entry_iter->second.category << "/write] " << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " updated.";
		Logger::instance().add(sstr.str());
	}
}


void Server::_handle_tpdo3(ucanopen::can_payload data)
{
	CobTpdo3 message = ucanopen::from_payload<CobTpdo3>(data);
	if ((message.syslog_message_id != 0) && (message.syslog_message_id < syslog_messages.size()))
	{
		Logger::instance().add(syslog_messages[message.syslog_message_id]);
	}
}


void Server::_handle_tpdo4(ucanopen::can_payload data)
{
	CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(data);
	_errors = message.errors;
	_warnings = message.warnings;
}

}

