#include "srmdrive_server.h"


namespace srmdrive {

Server::Server(std::shared_ptr<can::Socket> socket, ucanopen::NodeId node_id, const std::string& name)
        : ucanopen::Server(socket, node_id, name, object_dictionary)
        , ucanopen::SdoSubscriber(&sdo_service)
        , controller(this) {
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo1, std::chrono::milliseconds(200),
            [this](ucanopen::can_payload payload) { this->_handle_tpdo1(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo2, std::chrono::milliseconds(1200),
            [this](ucanopen::can_payload payload) { this->_handle_tpdo2(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo3, std::chrono::milliseconds(200),
            [this](ucanopen::can_payload payload) { this->_handle_tpdo3(payload); });
    tpdo_service.register_tpdo(ucanopen::TpdoType::tpdo4, std::chrono::milliseconds(200),
            [this](ucanopen::can_payload payload) { this->_handle_tpdo4(payload); });
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


void Server::_handle_tpdo3(const ucanopen::can_payload& payload) {
    CobTpdo3 message = ucanopen::from_payload<CobTpdo3>(payload);
    if ((message.syslog_message_id != 0) && (message.syslog_message_id < syslog_messages.size())) {
        Log() << syslog_messages[message.syslog_message_id] << '\n';
    }
}


void Server::_handle_tpdo4(const ucanopen::can_payload& payload) {
    CobTpdo4 message = ucanopen::from_payload<CobTpdo4>(payload);
    _errors = message.errors;
    _warnings = message.warnings;
}

}

