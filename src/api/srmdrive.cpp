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


void srmdrive_set_power_enabled(bool enabled) { srmdrive_server->set_power_enabled(enabled); }
void srmdrive_set_run_enabled(bool enabled) { srmdrive_server->set_run_enabled(enabled); }
void srmdrive_set_ctlmode(int mode) {
    if (mode == 0) {
        srmdrive_server->set_ctlmode(srmdrive::ControlMode::torque);
    } else {
        srmdrive_server->set_ctlmode(srmdrive::ControlMode::speed);
    }
}
void srmdrive_set_emergency_enabled(bool enabled) { srmdrive_server->set_emergency_enabled(enabled); }
void srmdrive_set_torque(double value_perunit) { srmdrive_server->set_torque(value_perunit); }
void srmdrive_set_speed(double value_rpm) { srmdrive_server->set_speed(value_rpm); }


void srmdrive_set_manual_fieldctl_enabled(bool enabled) { srmdrive_server->set_manual_fieldctl_enabled(enabled); }
void srmdrive_set_ctlloop(int ctlloop) {
    if (ctlloop == 0) {
        srmdrive_server->set_ctlloop(srmdrive::ControlLoopType::closed);
    } else {
        srmdrive_server->set_ctlloop(srmdrive::ControlLoopType::open);
    }
}
void srmdrive_set_field_current(double val) { srmdrive_server->set_field_current(val); }
void srmdrive_set_stator_current(double val_perunit) { srmdrive_server->set_stator_current(val_perunit); }


//----------------------------------------------------------------------------------------------------------------------
void srmdrive_tpdo1_get_drive_state(char* retbuf, int bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().drive_state.c_str(), bufsize-1);
}


bool srmdrive_tpdo1_get_run_status() {
    return srmdrive_server->tpdo1().run;
};


bool srmdrive_tpdo1_get_error_status() {
    return srmdrive_server->tpdo1().error;
}


bool srmdrive_tpdo1_get_warning_status() {
    return srmdrive_server->tpdo1().warning;
}


bool srmdrive_tpdo1_get_overheat_status() {
    return srmdrive_server->tpdo1().overheat;
}


void srmdrive_tpdo1_get_drive_reference(char* retbuf, int bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().ctlmode.c_str(), bufsize-1);
}


void srmdrive_tpdo1_get_drive_loop_type(char* retbuf, int bufsize) {
    retbuf[0] = '\0';
    strncat(retbuf, srmdrive_server->tpdo1().ctlloop.c_str(), bufsize-1);
}


int srmdrive_tpdo1_get_torque() {
    return srmdrive_server->tpdo1().torque;
}


int srmdrive_tpdo1_get_speed() {
    return srmdrive_server->tpdo1().speed;
}


//----------------------------------------------------------------------------------------------------------------------
unsigned int srmdrive_tpdo2_get_dc_voltage() {
    return srmdrive_server->tpdo2().dc_voltage;
}


unsigned int srmdrive_tpdo2_get_stator_current() {
    return srmdrive_server->tpdo2().stator_current;
}


float srmdrive_tpdo2_get_field_current() {
    return srmdrive_server->tpdo2().field_current;
}


unsigned int srmdrive_tpdo2_get_mech_power() {
    return srmdrive_server->tpdo2().mech_power;
}


bool srmdrive_tpdo2_get_manual_field_current_enabled() { 
    return srmdrive_server->tpdo2().manual_field_current;
}


//----------------------------------------------------------------------------------------------------------------------
int srmdrive_tpdo3_get_pwrmodule_temp() {
    return srmdrive_server->tpdo3().pwrmodule_temp;
}


int srmdrive_tpdo3_get_excmodule_temp() {
    return srmdrive_server->tpdo3().excmodule_temp;
}


int srmdrive_tpdo3_get_pcb_temp() {
    return srmdrive_server->tpdo3().pcb_temp;
}


int srmdrive_tpdo3_get_aw_temp() {
    return srmdrive_server->tpdo3().aw_temp;
}


int srmdrive_tpdo3_get_fw_temp() {
    return srmdrive_server->tpdo3().fw_temp;
} 

}

