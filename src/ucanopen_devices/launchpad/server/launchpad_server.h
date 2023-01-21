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
	std::array<float, 4> _clientValues;
	std::array<float, 4> _serverValues;

	uint32_t _errors = 0;
	uint16_t _warnings = 0;
protected:
	void _handleTpdo1(ucanopen::can_payload data) {}
	void _handleTpdo2(ucanopen::can_payload data) {}
	void _handleTpdo3(ucanopen::can_payload data) {}
	void _handleTpdo4(ucanopen::can_payload data);

	ucanopen::can_payload _createRpdo1()
	{
		static unsigned int counter = 0;
		CobRpdo1 message{.counter = counter, ._reserved = 0, .value = _serverValues[0]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobRpdo1>(message);
	}

	ucanopen::can_payload _createRpdo2()
	{
		static unsigned int counter = 0;
		CobRpdo2 message{.counter = counter, ._reserved = 0, .value = _serverValues[1]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobRpdo2>(message);
	}

	ucanopen::can_payload _createRpdo3()
	{
		static unsigned int counter = 0;
		CobRpdo3 message{.counter = counter, ._reserved = 0, .value = _serverValues[2]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobRpdo3>(message);
	}

	ucanopen::can_payload _createRpdo4()
	{
		static unsigned int counter = 0;
		CobRpdo4 message{.counter = counter, ._reserved = 0, .value = _serverValues[3]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobRpdo4>(message);
	}

	virtual void _handleTsdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionary::const_iterator entryIt,
			ucanopen::CobSdoData data) override final;
public:
	Server(const std::string& name,	ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket);

	void setClientValue(ucanopen::TpdoType tpdo, double value) { _clientValues[static_cast<size_t>(tpdo)] = value; }
	void setServerValue(ucanopen::RpdoType rpdo, double value) { _serverValues[static_cast<size_t>(rpdo)] = value; }

	uint32_t errors() const { return _errors; }
	uint16_t warnings() const { return _warnings; }

	ucanopen::can_payload createClientTpdo1()
	{
		static unsigned int counter = 0;
		CobClientTpdo1 message{.counter = counter, ._reserved = 0, .value = _clientValues[0]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobClientTpdo1>(message);
	}

	ucanopen::can_payload createClientTpdo2()
	{
		static unsigned int counter = 0;
		CobClientTpdo2 message{.counter = counter, ._reserved = 0, .value = _clientValues[1]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobClientTpdo2>(message);
	}

	ucanopen::can_payload createClientTpdo3()
	{
		static unsigned int counter = 0;
		CobClientTpdo3 message{.counter = counter, ._reserved = 0, .value = _clientValues[2]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobClientTpdo3>(message);
	}

	ucanopen::can_payload createClientTpdo4()
	{
		static unsigned int counter = 0;
		CobClientTpdo4 message{.counter = counter, ._reserved = 0, .value = _clientValues[3]};
		counter = (counter + 1) % 4;
		return ucanopen::to_payload<CobClientTpdo4>(message);
	}
};


} // namespace launchpad


