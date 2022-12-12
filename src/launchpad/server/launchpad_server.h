/**
 * @file launchpad_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/server/ucanopen_server.h"
#include "launchpad_def.h"


namespace launchpad {


extern const ucanopen::ObjectDictionaryType objectDictionary;


class Server : public ucanopen::IServer
{
protected:
	virtual void handleTpdo1(ucanopen::can_payload data) override final {}
	virtual void handleTpdo2(ucanopen::can_payload data) override final {}
	virtual void handleTpdo3(ucanopen::can_payload data) override final {}
	virtual void handleTpdo4(ucanopen::can_payload data) override final {}

	virtual ucanopen::can_payload createRpdo1() override final { return {}; }
	virtual ucanopen::can_payload createRpdo2() override final { return {}; }
	virtual ucanopen::can_payload createRpdo3() override final { return {}; }
	virtual ucanopen::can_payload createRpdo4() override final { return {}; }

	virtual void handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data) override final
	{
		// TODO
	}

public:
	Server(const std::string& name,
			ucanopen::NodeId nodeId,
			std::shared_ptr<can::Socket> socket,
			const ucanopen::ObjectDictionaryType& dictionary);
};


} // namespace launchpad


