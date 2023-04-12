#include <iostream>
#include <cassert>
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"


std::shared_ptr<srmdrive::Server> srmdrive_server;


namespace api {
void register_srmdrive_server(std::shared_ptr<srmdrive::Server> srmdrive_server_) {
    srmdrive_server = srmdrive_server_;
}
}


extern "C" {


void srmdrive_set_power_enabled(bool enabled) {
    if (enabled) {
        srmdrive_server->exec("drive", "ctl", "startup");
    } else {
        srmdrive_server->exec("drive", "ctl", "shutdown");
    }
}


void srmdrive_set_run_enabled(bool enabled) {
    srmdrive_server->set_run_enabled(enabled);
}


void srmdrive_set_emergency_enabled(bool enabled) {
    srmdrive_server->set_emergency_enabled(enabled);
}


void srmdrive_set_torque(double value_perunit) {
    srmdrive_server->set_torque(value_perunit);
}


void srmdrive_set_speed(double value_rpm) {
    srmdrive_server->set_speed(value_rpm);
}

}

