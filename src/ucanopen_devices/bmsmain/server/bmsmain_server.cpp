#include "bmsmain_server.h"


namespace bmsmain {

Server::Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket)
	: ucanopen::Server(name, node_id, socket, ucanopen::ObjectDictionary{}, ucanopen::ObjectDictionaryConfig{})
{
	tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(2000),
			[this](ucanopen::can_payload data) { this->_handle_tpdo1(data); });
}


void Server::_handle_tpdo1(ucanopen::can_payload data)
{
	static_assert(sizeof(CobTpdo1) == 8);
	CobTpdo1 message = ucanopen::from_payload<CobTpdo1>(data);

	int16_t current{message.current};
	_current = 0.1 * current;

	int8_t temp_min{message.temp_min};
	_temp_min = temp_min;

	int8_t temp_max{message.temp_max};
	_temp_max = temp_max;

	_charge = message.charge_percentage;
	_voltage = 0.1 * message.voltage;
}

}

