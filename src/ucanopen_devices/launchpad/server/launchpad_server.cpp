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

	tpdoService.registerTpdo(ucanopen::TpdoType::Tpdo1, std::chrono::milliseconds(60),
			[this](ucanopen::can_payload data) { this->_handleTpdo1(data); });
	tpdoService.registerTpdo(ucanopen::TpdoType::Tpdo2, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handleTpdo2(data); });
	tpdoService.registerTpdo(ucanopen::TpdoType::Tpdo3, std::chrono::milliseconds(1100),
			[this](ucanopen::can_payload data) { this->_handleTpdo3(data); });
	tpdoService.registerTpdo(ucanopen::TpdoType::Tpdo4, std::chrono::milliseconds(110),
			[this](ucanopen::can_payload data) { this->_handleTpdo4(data); });

	rpdoService.registerRpdo(ucanopen::RpdoType::Rpdo1, std::chrono::milliseconds(25),
			[this](){ return this->_createRpdo1(); });
	rpdoService.registerRpdo(ucanopen::RpdoType::Rpdo2, std::chrono::milliseconds(50),
			[this](){ return this->_createRpdo2(); });
	rpdoService.registerRpdo(ucanopen::RpdoType::Rpdo3, std::chrono::milliseconds(100),
			[this](){ return this->_createRpdo3(); });
	rpdoService.registerRpdo(ucanopen::RpdoType::Rpdo4, std::chrono::milliseconds(500),
			[this](){ return this->_createRpdo4(); });
}


///
///
///
void Server::_handleTsdo(ucanopen::SdoType sdoType,
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
	else if (entryIt->second.category == watchService.watchCategory && entryIt->second.dataType == ucanopen::OD_ENUM16)
	{
		
	}
	else if (entryIt->second.category == configService.configCategory && sdoType == ucanopen::SdoType::ResponseToRead)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/read] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " = " << data.toString(entryIt->second.dataType);
		Logger::instance().add(sstr.str());
	}
	else if (entryIt->second.category == configService.configCategory && sdoType == ucanopen::SdoType::ResponseToWrite)
	{
		std::stringstream sstr;
		sstr << "[" << entryIt->second.category << "/write] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " updated.";
		Logger::instance().add(sstr.str());
	}
}


} // namespace launchpad


