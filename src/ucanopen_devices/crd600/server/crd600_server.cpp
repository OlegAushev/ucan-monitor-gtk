#include "crd600_server.h"


namespace crd600 {


Server::Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, node_id, socket, object_dictionary, object_dictionary_config)
{
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(60),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo1(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(60),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo2(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(120),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo3(payload); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(120),
			[this](ucanopen::can_payload payload) { this->_handle_tpdo4(payload); });

	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo1, std::chrono::milliseconds(25),
			[this](){ return this->_create_rpdo1(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo2, std::chrono::milliseconds(50),
			[this](){ return this->_create_rpdo2(); });
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


void Server::_handle_tpdo1(const ucanopen::can_payload& payload)
{
	CobTpdo1 message = ucanopen::from_payload<CobTpdo1>(payload);
	tpdo1.status_drive1_run = message.status_drive1_run;
	tpdo1.status_drive2_run = message.status_drive2_run;
	tpdo1.status_error = message.status_error;
	tpdo1.status_warning = message.status_warning;
	tpdo1.status_overheat = message.status_overheat;
	tpdo1.drive1_ref = (message.drive1_ref == 0) ? "speed" : "torque";
	tpdo1.drive2_ref = (message.drive2_ref == 0) ? "speed" : "torque";
	tpdo1.control_loop_type = (message.control_loop_type == 0) ? "open" : "closed";

	auto get_drive_state = [](unsigned int id)
	{
		if (id >= drive_states.size()) return std::string("n/a");
		return drive_states[id];
	};

	tpdo1.drive1_state = get_drive_state(message.drive1_state);
	tpdo1.drive2_state = get_drive_state(message.drive2_state);
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload)
{
	CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(payload);
	_errors = message.errors;
	_warnings = message.warnings;
}

}

