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
#include "logger/logger.h"


namespace launchpad {


extern const ucanopen::ObjectDictionary objectDictionary;
extern const ucanopen::ObjectDictionaryConfig objectDictionaryConfig;


class Server : public ucanopen::Server
{
private:
	std::array<double, 4> _clientValues;
	std::array<double, 4> _serverValues;
protected:
	void _handleTpdo1(ucanopen::can_payload data) {}
	void _handleTpdo2(ucanopen::can_payload data) {}
	void _handleTpdo3(ucanopen::can_payload data) {}
	void _handleTpdo4(ucanopen::can_payload data) {}

	ucanopen::can_payload _createRpdo1() { return ucanopen::toPayload<CobRpdo1>(CobRpdo1(_serverValues[0])); }
	ucanopen::can_payload _createRpdo2() { return ucanopen::toPayload<CobRpdo2>(CobRpdo2(_serverValues[1])); }
	ucanopen::can_payload _createRpdo3() { return ucanopen::toPayload<CobRpdo3>(CobRpdo3(_serverValues[2])); }
	ucanopen::can_payload _createRpdo4() { return ucanopen::toPayload<CobRpdo4>(CobRpdo4(_serverValues[3])); }

	virtual void _handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data) override final;
public:
	Server(const std::string& name,	ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket);

	void setClientValue(ucanopen::TpdoType tpdo, double value) { _clientValues[static_cast<size_t>(tpdo)] = value; }
	void setServerValue(ucanopen::RpdoType rpdo, double value) { _serverValues[static_cast<size_t>(rpdo)] = value; }

	ucanopen::can_payload createTpdo1() { return ucanopen::toPayload<CobClientTpdo1>(CobClientTpdo1(_clientValues[0])); }
	ucanopen::can_payload createTpdo2() { return ucanopen::toPayload<CobClientTpdo2>(CobClientTpdo2(_clientValues[1])); }
	ucanopen::can_payload createTpdo3() { return ucanopen::toPayload<CobClientTpdo3>(CobClientTpdo3(_clientValues[2])); }
	ucanopen::can_payload createTpdo4() { return ucanopen::toPayload<CobClientTpdo4>(CobClientTpdo4(_clientValues[3])); }
};


} // namespace launchpad


