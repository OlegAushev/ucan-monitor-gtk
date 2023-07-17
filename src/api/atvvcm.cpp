#include <iostream>
#include <cassert>
#include "ucanopen_devices/atvvcm/server/atvvcm_server.h"


std::shared_ptr<atvvcm::Server> atvvcm_server;


namespace api {
void register_atvvcm_server(std::shared_ptr<atvvcm::Server> atvvcm_server_) {
    atvvcm_server = atvvcm_server_;
}
}


extern "C" {

void atvvcm_set_client_value(unsigned int tpdo_num, double value) {
    assert(tpdo_num <= 3);
    atvvcm_server->set_client_value(static_cast<ucanopen::TpdoType>(tpdo_num), value);
}


void atvvcm_set_server_value(unsigned int rpdo_num, double value) {
    assert(rpdo_num <= 3);
    atvvcm_server->set_server_value(static_cast<ucanopen::RpdoType>(rpdo_num), value);
}


bool atvvcm_get_pdm_contactor_state(unsigned int contactor) {
    assert(contactor <= atvvcm::pdm_contactor_count);
    return atvvcm_server->pdm_contactor_state(static_cast<atvvcm::PdmContactor>(contactor));
}

}

