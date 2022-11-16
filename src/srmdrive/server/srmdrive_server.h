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


namespace srmdrive {


extern const ucanopen::ObjectDictionaryType OBJECT_DICTIONARY;


class Server : public ucanopen::IServer
{
private:

protected:
	virtual void handleTpdo1(ucanopen::can_payload data) override {}
	virtual void handleTpdo2(ucanopen::can_payload data) override {}
	virtual void handleTpdo3(ucanopen::can_payload data) override {}
	virtual void handleTpdo4(ucanopen::can_payload data) override {}

	virtual ucanopen::can_payload createRpdo1() override { return {}; }
	virtual ucanopen::can_payload createRpdo2() override { return {}; }
	virtual ucanopen::can_payload createRpdo3() override { return {}; }
	virtual ucanopen::can_payload createRpdo4() override { return {}; }

	virtual void handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data) override
	{
		observer.handleSdo(sdoType, entryIt, data);
	}

public:
	Controller controller;
	Observer observer;

	Server(ucanopen::NodeId nodeId,
			std::shared_ptr<can::Socket> socket,
			const ucanopen::ObjectDictionaryType& dictionary);

};


} // namespace srmdrive

