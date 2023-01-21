/**
 * @file srmdrive_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "srmdrive_server.h"


namespace srmdrive {


///
///
///
Server::Server(const std::string& name, ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, nodeId, socket, objectDictionary, objectDictionaryConfig)
	, controller(this)
{
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handleTpdo1(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(1200),
			[this](ucanopen::can_payload data) { this->_handleTpdo2(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handleTpdo3(data); });
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(200),
			[this](ucanopen::can_payload data) { this->_handleTpdo4(data); });
}


///
///
///
void Server::_handle_tsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data)
{
	if (entryIt->second.category == watch_service.watch_category && entryIt->second.data_type == ucanopen::OD_ENUM16)
	{
		if (entryIt->second.name == "DRIVE_STATE" && data.u32() < driveStates.size())
		{
			watch_service.set_value(entryIt->second.name, driveStates[data.u32()]);
		}
	}
	else if (entryIt->second.category == config_service.config_category && sdoType == ucanopen::SdoType::response_to_read)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/read] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " = " << data.to_string(entryIt->second.data_type);
		Logger::instance().add(sstr.str());
	}
	else if (entryIt->second.category == config_service.config_category && sdoType == ucanopen::SdoType::response_to_write)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/write] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " updated.";
		Logger::instance().add(sstr.str());
	}
}


///
///
///
void Server::_handleTpdo3(ucanopen::can_payload data)
{
	CobTpdo3 message = ucanopen::from_payload<CobTpdo3>(data);
	if ((message.syslogMessageId != 0) && (message.syslogMessageId < syslogMessages.size()))
	{
		Logger::instance().add(syslogMessages[message.syslogMessageId]);
	}
}


///
///
///
void Server::_handleTpdo4(ucanopen::can_payload data)
{
	CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(data);
	_errors = message.errors;
	_warnings = message.warnings;
}

}


