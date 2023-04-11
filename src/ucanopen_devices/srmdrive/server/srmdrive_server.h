#pragma once


#include "ucanopen/server/server.h"
#include "srmdrive_def.h"
#include "../controller/srmdrive_controller.h"
#include "log/log.h"


namespace srmdrive {

extern const ucanopen::ObjectDictionary object_dictionary;
extern const ucanopen::ObjectDictionaryConfig object_dictionary_config;


class Server : public ucanopen::Server, public ucanopen::SdoSubscriber {
private:
    uint32_t _errors = 0;
    uint16_t _warnings = 0;
public:
    Controller controller;
public:
    Server(std::shared_ptr<can::Socket> socket, ucanopen::NodeId node_id, const std::string& name);

    uint32_t errors() const { return _errors; }
    uint16_t warnings() const { return _warnings; }
protected:
    virtual ucanopen::FrameHandlingStatus handle_sdo(ucanopen::ODEntryIter entry,
                                                     ucanopen::SdoType sdo_type,
                                                     ucanopen::ExpeditedSdoData data) override final;
private:
    void _handle_tpdo1(const ucanopen::can_payload& payload);
    void _handle_tpdo2(const ucanopen::can_payload& payload){}
    void _handle_tpdo3(const ucanopen::can_payload& payload){}
    void _handle_tpdo4(const ucanopen::can_payload& payload);

    ucanopen::can_payload _create_rpdo1() { return {}; }
    ucanopen::can_payload _create_rpdo2() { return {}; }

    struct Tpdo1 {
        bool status_run;
        bool status_error;
        bool status_warning;
        bool status_overheat;
        std::string reference;
        std::string control_loop;
        std::string drive_state;
        std::string dc_voltage;
        std::string torque;
        std::string speed;
    } tpdo1;
};

} // namespace srmdrive

