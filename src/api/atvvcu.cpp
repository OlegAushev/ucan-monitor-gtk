#include <iostream>
#include <cassert>
#include "ucanopen_devices/atvvcu/server/atvvcu_server.h"


std::shared_ptr<atvvcu::Server> atvvcu_server;


namespace api {
void register_atvvcu_server(std::shared_ptr<atvvcu::Server> atvvcu_server_) {
    atvvcu_server = atvvcu_server_;
}
}


extern "C" {

void atvvcu_set_client_value(unsigned int tpdo_num, double value) {
    assert(tpdo_num <= 3);
    atvvcu_server->set_client_value(static_cast<ucanopen::TpdoType>(tpdo_num), value);
}


void atvvcu_set_server_value(unsigned int rpdo_num, double value) {
    assert(rpdo_num <= 3);
    atvvcu_server->set_server_value(static_cast<ucanopen::RpdoType>(rpdo_num), value);
}


bool atvvcu_get_pdm_contactor_state(unsigned int contactor) {
    assert(contactor <= atvvcu::pdm_contactor_count);
    return atvvcu_server->pdm_contactor_state(static_cast<atvvcu::PdmContactor>(contactor));
}

}

