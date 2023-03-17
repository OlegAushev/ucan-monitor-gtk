#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"


std::shared_ptr<bmsmain::Server> bmsmain_server;


namespace api {
void register_bmsmain_server(std::shared_ptr<bmsmain::Server> bmsmain_server_) {
    bmsmain_server = bmsmain_server_;
}
}


extern "C" {

double bmsmain_tpdo1_get_voltage() {
    return bmsmain_server->voltage();
}

double bmsmain_tpdo1_get_current() {
    return bmsmain_server->current();
}

double bmsmain_tpdo1_get_charge() {
    return bmsmain_server->charge();
}

double bmsmain_tpdo1_get_temp_max() {
    return bmsmain_server->temp_max();
}

double bmsmain_tpdo1_get_temp_min() {
    return bmsmain_server->temp_min();
}

}

