/**
 * @file bmsmain_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "bmsmain_server.h"


namespace bmsmain {


///
///
///
Server::Server(const std::string& name, ucanopen::NodeId nodeId, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, nodeId, socket, ucanopen::ObjectDictionary{}, ucanopen::ObjectDictionaryConfig{})
{
	tpdo_service.registerTpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(2000),
			[this](ucanopen::can_payload data) { this->_handleTpdo1(data); });
}


///
///
///
void Server::_handleTpdo1(ucanopen::can_payload data)
{
	static_assert(sizeof(CobTpdo1) == 8);
	CobTpdo1 message = ucanopen::from_payload<CobTpdo1>(data);

	int16_t current{message.current};
	_current = 0.1 * current;

	int8_t tempMin{message.tempMin};
	_tempMin = tempMin;

	int8_t tempMax{message.tempMax};
	_tempMax = tempMax;

	_charge = message.chargePercentage;
	_voltage = 0.1 * message.voltage;
}


}


