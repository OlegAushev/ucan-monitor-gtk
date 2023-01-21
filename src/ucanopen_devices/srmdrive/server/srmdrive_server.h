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
#include "logger/logger.h"


namespace srmdrive {


extern const ucanopen::ObjectDictionary objectDictionary;
extern const ucanopen::ObjectDictionaryConfig objectDictionaryConfig;


class Server : public ucanopen::Server
{
protected:
	void _handleTpdo1(ucanopen::can_payload data) {}
	void _handleTpdo2(ucanopen::can_payload data) {}
	void _handleTpdo3(ucanopen::can_payload data);
	void _handleTpdo4(ucanopen::can_payload data);

	virtual void _handle_tsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data) override final;
private:
	uint32_t _errors = 0;
	uint16_t _warnings = 0;

public:
	Controller controller;

	Server(const std::string& name, ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket);

	uint32_t errors() const { return _errors; }
	uint16_t warnings() const { return _warnings; }
};


} // namespace srmdrive

