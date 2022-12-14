/**
 * @file srmdrive_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/server/ucanopen_server.h"
#include "srmdrive_def.h"
#include "../controller/srmdrive_controller.h"
#include "../observer/srmdrive_observer.h"
#include "logger/logger.h"


namespace srmdrive {


extern const ucanopen::ObjectDictionaryType objectDictionary;


class Server : public ucanopen::IServer
{
private:

protected:
	virtual void handleTpdo1(ucanopen::can_payload data) override final {}
	virtual void handleTpdo2(ucanopen::can_payload data) override final {}
	virtual void handleTpdo3(ucanopen::can_payload data) override final;
	virtual void handleTpdo4(ucanopen::can_payload data) override final {}

	virtual ucanopen::can_payload createRpdo1() override final { return {}; }
	virtual ucanopen::can_payload createRpdo2() override final { return {}; }
	virtual ucanopen::can_payload createRpdo3() override final { return {}; }
	virtual ucanopen::can_payload createRpdo4() override final { return {}; }

	virtual void handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data) override final
	{
		observer.handleSdo(sdoType, entryIt, data);
	}

public:
	Controller controller;
	Observer observer;

	Server(const std::string& name,
			ucanopen::NodeId nodeId,
			std::shared_ptr<can::Socket> socket,
			const ucanopen::ObjectDictionaryType& dictionary);

};


} // namespace srmdrive

