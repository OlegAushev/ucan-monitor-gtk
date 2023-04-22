#include "srmdrive_server.h"


namespace srmdrive {

Server::Server(std::shared_ptr<can::Socket> socket, ucanopen::NodeId node_id, const std::string& name)
        : ucanopen::Server(socket, node_id, name, object_dictionary)
        , ucanopen::SdoSubscriber(sdo_service) {
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(1000),
                               [this](ucanopen::can_payload payload) { this->_handle_tpdo1(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(1000),
                               [this](ucanopen::can_payload payload) { this->_handle_tpdo2(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(1000),
                               [this](ucanopen::can_payload payload) { this->_handle_tpdo3(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(1000),
                               [this](ucanopen::can_payload payload) { this->_handle_tpdo4(payload); });

    rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo1, std::chrono::milliseconds(100),
                               [this](){ return this->_create_rpdo1(); });
    rpdo_service.register_rpdo(ucanopen::RpdoType::rpdo2, std::chrono::milliseconds(100),
                               [this](){ return this->_create_rpdo2(); });
}


ucanopen::FrameHandlingStatus Server::handle_sdo(ucanopen::ODEntryIter entry,
                                                 ucanopen::SdoType sdo_type,
                                                 ucanopen::ExpeditedSdoData data) {
    if (entry->second.name == "syslog_message") {
        auto message_id = data.u32();
        if ((message_id != 0) && (message_id < syslog_messages.size())) {
            Log() << syslog_messages[message_id] << '\n';
        }
    } else if (entry->second.category == _dictionary.config.config_category && entry->second.type == ucanopen::OD_ENUM16) {
        
    }

    return ucanopen::FrameHandlingStatus::success;
}


//----------------------------------------------------------------------------------------------------------------------
void Server::_handle_tpdo1(const ucanopen::can_payload& payload){
    CobTpdo1 tpdo = ucanopen::from_payload<CobTpdo1>(payload);
    _tpdo1.status_run = tpdo.status_run;
    _tpdo1.status_error = tpdo.status_error;
    _tpdo1.status_warning = tpdo.status_warning;
    _tpdo1.status_overheat = tpdo.status_overheat;
    _tpdo1.reference = (tpdo.reference == 0) ? "torque" : "speed";
    _tpdo1.control_loop = (tpdo.control_loop == 0) ? "closed" : "open";

    if (tpdo.drive_state >= drive_states.size()) {
        _tpdo1.drive_state = "n/a";
    } else {
        _tpdo1.drive_state = drive_states[tpdo.drive_state];
    }

    _tpdo1.dc_voltage = tpdo.dc_voltage;
    _tpdo1.torque = tpdo.torque;
    _tpdo1.speed = tpdo.speed;
}


void Server::_handle_tpdo2(const ucanopen::can_payload& payload) {
    CobTpdo2 tpdo = ucanopen::from_payload<CobTpdo2>(payload);
    _tpdo2.stator_current = tpdo.stator_current;
    _tpdo2.field_current = tpdo.field_current;
    _tpdo2.out_voltage = tpdo.out_voltage;
    _tpdo2.mech_power = tpdo.mech_power;
    _tpdo2.elec_power = tpdo.elec_power;
}


void Server::_handle_tpdo3(const ucanopen::can_payload& payload) {
    CobTpdo3 tpdo = ucanopen::from_payload<CobTpdo3>(payload);
    _tpdo3.pwrmodule_temp = int(tpdo.pwrmodule_temp) - 40;
    _tpdo3.excmodule_temp = int(tpdo.excmodule_temp) - 40;
    _tpdo3.pcb_temp = int(tpdo.pcb_temp) - 40;
    _tpdo3.aw_temp = int(tpdo.aw_temp) - 40;
    _tpdo3.fw_temp = int(tpdo.fw_temp) - 40;
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload){
    CobTpdo4 tpdo = ucanopen::from_payload<CobTpdo4>(payload);
    _errors = tpdo.errors;
    _warnings = tpdo.warnings;
}


//----------------------------------------------------------------------------------------------------------------------
ucanopen::can_payload Server::_create_rpdo1() {
    static unsigned int counter = 0;
    CobRpdo1 rpdo;

    rpdo.counter = counter;
    rpdo.run = _run_enabled;
    rpdo.emergency_stop = _emergency_enabled;

    counter = (counter + 1) & 0x3;
    return ucanopen::to_payload<CobRpdo1>(rpdo);
}


ucanopen::can_payload Server::_create_rpdo2() {
    static unsigned int counter = 0;
    CobRpdo2 rpdo;

    rpdo.counter = counter;
    rpdo.torque_ref = 32767.0f * _torque_perunit_ref;
    rpdo.speed_ref = _speed_rpm_ref;

    counter = (counter + 1) & 0x3;
    return ucanopen::to_payload<CobRpdo2>(rpdo);
}



}

