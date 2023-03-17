#include <iostream>
#include <cassert>
#include "ucanopen_devices/launchpad/server/launchpad_server.h"


std::shared_ptr<launchpad::Server> launchpad_server;


namespace api {
void register_launchpad_server(std::shared_ptr<launchpad::Server> launchpad_server_) {
    launchpad_server = launchpad_server_;
}
}


extern "C" {

void launchpad_set_client_value(unsigned int tpdo_num, double value) {
    assert(tpdo_num <= 3);
    launchpad_server->set_client_value(static_cast<ucanopen::TpdoType>(tpdo_num), value);
}


void launchpad_set_server_value(unsigned int rpdo_num, double value) {
    assert(rpdo_num <= 3);
    launchpad_server->set_server_value(static_cast<ucanopen::RpdoType>(rpdo_num), value);
}

}

