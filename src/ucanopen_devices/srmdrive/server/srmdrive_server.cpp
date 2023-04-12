#include "srmdrive_server.h"


namespace srmdrive {

Server::Server(std::shared_ptr<can::Socket> socket, ucanopen::NodeId node_id, const std::string& name)
        : ucanopen::Server(socket, node_id, name, object_dictionary)
        , ucanopen::SdoSubscriber(&sdo_service) {
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

    _tpdo1.dc_voltage = std::to_string(tpdo.dc_voltage);
    _tpdo1.torque = std::to_string(tpdo.torque);
    _tpdo1.speed = std::to_string(tpdo.speed);
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload){
    CobTpdo4 tpdo = ucanopen::from_payload<CobTpdo4>(payload);
    _errors = tpdo.errors;
    _warnings = tpdo.warnings;
}


// void Server::_handle_tpdo3(const ucanopen::can_payload& payload) {
//     CobTpdo3 message = ucanopen::from_payload<CobTpdo3>(payload);
//     if ((message.syslog_message_id != 0) && (message.syslog_message_id < syslog_messages.size())) {
//         Log() << syslog_messages[message.syslog_message_id] << '\n';
//     }
// }


// void Server::_handle_tpdo4(const ucanopen::can_payload& payload) {
//     CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(payload);
//     _errors = message.errors;
//     _warnings = message.warnings;
// }


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

