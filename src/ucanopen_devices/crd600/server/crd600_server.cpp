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

	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo1, std::chrono::milliseconds(50),
			[this](){ return this->_create_rpdo1(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo2, std::chrono::milliseconds(50),
			[this](){ return this->_create_rpdo2(); });
	rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo3, std::chrono::milliseconds(50),
			[this](){ return this->_create_rpdo3(); });
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
	CobTpdo1 tpdo = ucanopen::from_payload<CobTpdo1>(payload);
	tpdo1.status_drive1_run = tpdo.status_drive1_run;
	tpdo1.status_drive2_run = tpdo.status_drive2_run;
	tpdo1.status_error = tpdo.status_error;
	tpdo1.status_warning = tpdo.status_warning;
	tpdo1.status_overheat = tpdo.status_overheat;
	tpdo1.drive1_ref = (tpdo.drive1_ref == 0) ? "speed" : "torque";
	tpdo1.drive2_ref = (tpdo.drive2_ref == 0) ? "speed" : "torque";
	tpdo1.control_loop_type = (tpdo.control_loop_type == 0) ? "open" : "closed";

	auto get_drive_state = [](unsigned int id)
	{
		if (id >= drive_states.size()) return std::string("n/a");
		return drive_states[id];
	};

	tpdo1.drive1_state = get_drive_state(tpdo.drive1_state);
	tpdo1.drive2_state = get_drive_state(tpdo.drive2_state);
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload)
{
	CobTpdo4 tpdo = ucanopen::from_payload<CobTpdo4>(payload);
	_errors = tpdo.errors;
	_warnings = tpdo.warnings;
}


ucanopen::can_payload Server::_create_rpdo1()
{
	static unsigned int counter = 0;
	CobRpdo1 rpdo{};

	rpdo.counter = counter;
	rpdo.drive1_run = drive1_run;
	rpdo.drive2_run = drive2_run;
	rpdo.emergency_stop = emergency_stop;

	counter = (counter + 1) % 4;
	return ucanopen::to_payload<CobRpdo1>(rpdo);
}


ucanopen::can_payload Server::_create_rpdo2()
{
	static unsigned int counter = 0;
	CobRpdo2 rpdo{};

	rpdo.counter = counter;
	rpdo.drive1_speed_ref = drive1_speed_ref;
	rpdo.drive1_torque_ref = drive1_torque_ref;

	counter = (counter + 1) % 4;
	return ucanopen::to_payload<CobRpdo2>(rpdo);
}

ucanopen::can_payload Server::_create_rpdo3()
{
	static unsigned int counter = 0;
	CobRpdo3 rpdo{};

	rpdo.counter = counter;
	rpdo.drive2_speed_ref = drive2_speed_ref;
	rpdo.drive2_torque_ref = drive2_torque_ref;

	counter = (counter + 1) % 4;
	return ucanopen::to_payload<CobRpdo3>(rpdo);
}

}

