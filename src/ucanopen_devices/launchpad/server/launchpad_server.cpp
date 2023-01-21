/**
 * @file launchpad_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "launchpad_server.h"


namespace launchpad {


///
///
///
Server::Server(const std::string& name, ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, nodeId, socket, objectDictionary, objectDictionaryConfig)
{
	_clientValues.fill(0);
	_serverValues.fill(0);

	tpdo_service.registerTpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(60),
			[this](ucanopen::can_payload data) { this->_handleTpdo1(data); });
	tpdo_service.registerTpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handleTpdo2(data); });
	tpdo_service.registerTpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(1100),
			[this](ucanopen::can_payload data) { this->_handleTpdo3(data); });
	tpdo_service.registerTpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handleTpdo4(data); });

	rpdo_service.registerRpdo(ucanopen::RpdoType::rpdo1, std::chrono::milliseconds(25),
			[this](){ return this->_createRpdo1(); });
	rpdo_service.registerRpdo(ucanopen::RpdoType::rpdo2, std::chrono::milliseconds(50),
			[this](){ return this->_createRpdo2(); });
	rpdo_service.registerRpdo(ucanopen::RpdoType::rpdo3, std::chrono::milliseconds(100),
			[this](){ return this->_createRpdo3(); });
	rpdo_service.registerRpdo(ucanopen::RpdoType::rpdo4, std::chrono::milliseconds(500),
			[this](){ return this->_createRpdo4(); });
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


///
///
///
void Server::_handle_tsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data)
{
	if (entryIt->second.name == "syslog_message")
	{
		auto messageId = data.u32();
		if ((messageId != 0) && (messageId < syslogMessages.size()))
		{
			Logger::instance().add(syslogMessages[messageId]);
		}
	}
	else if (entryIt->second.category == watch_service.watchCategory && entryIt->second.data_type == ucanopen::OD_ENUM16)
	{
		
	}
	else if (entryIt->second.category == config_service.configCategory && sdoType == ucanopen::SdoType::response_to_read)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/read] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " = " << data.to_string(entryIt->second.data_type);
		Logger::instance().add(sstr.str());
	}
	else if (entryIt->second.category == config_service.configCategory && sdoType == ucanopen::SdoType::response_to_write)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/write] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " updated.";
		Logger::instance().add(sstr.str());
	}
}


} // namespace launchpad


