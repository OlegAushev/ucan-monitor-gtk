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


//----------------------------------------------------------------------------------------------------------------------
void srmdrive_tpdo1_get_drive_state(char* retbuf, size_t bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().drive_state.c_str(), bufsize-1);
}


bool srmdrive_tpdo1_get_run_status() {
    return srmdrive_server->tpdo1().status_run;
};


bool srmdrive_tpdo1_get_error_status() {
    return srmdrive_server->tpdo1().status_error;
}


bool srmdrive_tpdo1_get_warning_status() {
    return srmdrive_server->tpdo1().status_warning;
}


bool srmdrive_tpdo1_get_overheat_status() {
    return srmdrive_server->tpdo1().status_overheat;
}


void srmdrive_tpdo1_get_drive_reference(char* retbuf, size_t bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().reference.c_str(), bufsize-1);
}


void srmdrive_tpdo1_get_drive_loop_type(char* retbuf, size_t bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().control_loop.c_str(), bufsize-1);
}


unsigned int srmdrive_tpdo1_get_dc_voltage() {
    return srmdrive_server->tpdo1().dc_voltage;
}


int srmdrive_tpdo1_get_torque() {
    return srmdrive_server->tpdo1().torque;
}


int srmdrive_tpdo1_get_speed() {
    return srmdrive_server->tpdo1().speed;
}

}

