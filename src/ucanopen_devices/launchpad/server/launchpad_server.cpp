#include "launchpad_server.h"


namespace launchpad {

Server::Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, node_id, socket, object_dictionary, object_dictionary_config)
{
	_client_values.fill(0);
	_server_values.fill(0);

	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(60),
			[this](ucanopen::can_payload data) { this->_handle_tpdo1(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handle_tpdo2(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(1100),
			[this](ucanopen::can_payload data) { this->_handle_tpdo3(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handle_tpdo4(data); });

	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo1, std::chrono::milliseconds(25),
			[this](){ return this->_create_rpdo1(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo2, std::chrono::milliseconds(50),
			[this](){ return this->_create_rpdo2(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo3, std::chrono::milliseconds(100),
			[this](){ return this->_create_rpdo3(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo4, std::chrono::milliseconds(500),
			[this](){ return this->_create_rpdo4(); });
}


void Server::_handle_tpdo4(ucanopen::can_payload data)
{
	CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(data);
	_errors = message.errors;
	_warnings = message.warnings;
}


void Server::_handle_tsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entry_iter,
			ucanopen::CobSdoData data)
{
	if (entry_iter->second.name == "syslog_message")
	{
		auto message_id = data.u32();
		if ((message_id != 0) && (message_id < syslog_messages.size()))
		{
			Logger::instance().add(syslog_messages[message_id]);
		}
	}
	else if (entry_iter->second.category == watch_service.watch_category && entry_iter->second.data_type == ucanopen::OD_ENUM16)
	{
		
	}
	else if (entry_iter->second.category == config_service.config_category && sdoType == ucanopen::SdoType::response_to_read)
	{
		std::stringstream sstr;
		sstr << "[" << entry_iter->second.category << "/read] " << entry_iter->second.subcategory << "::" << entry_iter->second.name
				<< " = " << data.to_string(entry_iter->second.data_type);
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

} // namespace launchpad

