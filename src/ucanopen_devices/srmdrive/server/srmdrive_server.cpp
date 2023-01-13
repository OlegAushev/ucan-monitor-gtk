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
	: IServer(name, nodeId, socket, objectDictionary, objectDictionaryConfig)
	, controller(this)
{
	_registerTpdo(ucanopen::TpdoType::Tpdo1, std::chrono::milliseconds(200));
	_registerTpdo(ucanopen::TpdoType::Tpdo2, std::chrono::milliseconds(1200));
	_registerTpdo(ucanopen::TpdoType::Tpdo3, std::chrono::milliseconds(200));
	_registerTpdo(ucanopen::TpdoType::Tpdo4, std::chrono::milliseconds(200));
}


///
///
///
void Server::_handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data)
{
	if (entryIt->second.category == watchService.dictionaryCategory && entryIt->second.dataType == ucanopen::OD_ENUM16)
	{
		if (entryIt->second.name == "DRIVE_STATE" && data.u32() < driveStates.size())
		{
			watchService.setValue(entryIt->second.name, driveStates[data.u32()]);
		}
	}
	else if (entryIt->second.category == configCategory && sdoType == ucanopen::SdoType::ResponseToRead)
	{
		std::stringstream sstr;
		sstr << "[" << configCategory << "/read] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " = " << data.toString(entryIt->second.dataType);
		Logger::instance().add(sstr.str());
	}
	else if (entryIt->second.category == configCategory && sdoType == ucanopen::SdoType::ResponseToWrite)
	{
		std::stringstream sstr;
		sstr << "[" << configCategory << "/write] " << entryIt->second.subcategory << "::" << entryIt->second.name
				<< " updated.";
		Logger::instance().add(sstr.str());
	}
}


///
///
///
void Server::_handleTpdo3(ucanopen::can_payload data)
{
	CobTpdo3 message = ucanopen::fromPayload<CobTpdo3>(data);
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
	CobTpdo4 message = ucanopen::fromPayload<CobTpdo4>(data);
	_errors = message.errors;
	_warnings = message.warnings;
}

}


