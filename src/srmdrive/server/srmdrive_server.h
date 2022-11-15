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
	virtual void handleTpdo1(std::array<uint8_t, 8> data) override {}
	virtual void handleTpdo2(std::array<uint8_t, 8> data) override {}
	virtual void handleTpdo3(std::array<uint8_t, 8> data) override {}
	virtual void handleTpdo4(std::array<uint8_t, 8> data) override {}

	virtual std::array<uint8_t, 8> createRpdo1() override { return {}; }
	virtual std::array<uint8_t, 8> createRpdo2() override { return {}; }
	virtual std::array<uint8_t, 8> createRpdo3() override { return {}; }
	virtual std::array<uint8_t, 8> createRpdo4() override { return {}; }

	virtual void handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data) override
	{
		observer.processSdo(sdoType, entryIt, data);
	}

public:
	Controller controller;
	Observer observer;

	Server(ucanopen::NodeId nodeId,
			std::shared_ptr<can::Socket> socket,
			const ucanopen::ObjectDictionaryType& dictionary);

};


} // namespace srmdrive

