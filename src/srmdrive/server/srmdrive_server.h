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
#include "objectdictionary/objectdictionary.h"
#include "../controller/srmdrive_controller.h"
#include "../observer/srmdrive_observer.h"


namespace srmdrive {


class Server : public ucanopen::IServer
{
private:

protected:
	virtual void processTpdo1(std::array<uint8_t, 8> data) override {}
	virtual void processTpdo2(std::array<uint8_t, 8> data) override {}
	virtual void processTpdo3(std::array<uint8_t, 8> data) override {}
	virtual void processTpdo4(std::array<uint8_t, 8> data) override {}

	virtual std::array<uint8_t, 8> makeRpdo1() override { return {}; }
	virtual std::array<uint8_t, 8> makeRpdo2() override { return {}; }
	virtual std::array<uint8_t, 8> makeRpdo3() override { return {}; }
	virtual std::array<uint8_t, 8> makeRpdo4() override { return {}; }

	virtual void processTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data) override
	{
		observer.processSdo(sdoType, entryIt, data);
	}

public:
	Controller controller;
	Observer observer;

	Server(ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket, const ucanopen::ObjectDictionaryType& dictionary)
		: IServer(nodeId, socket, dictionary)
		, controller(this)
		, observer(this)
	{

	}

};


} // namespace srmdrive

