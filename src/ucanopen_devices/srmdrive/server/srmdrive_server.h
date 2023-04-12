#pragma once


#include "srmdrive_def.h"
#include "ucanopen/server/server.h"
#include "log/log.h"
#include <algorithm>


namespace srmdrive {

extern const ucanopen::ObjectDictionary object_dictionary;
extern const ucanopen::ObjectDictionaryConfig object_dictionary_config;


class Server : public ucanopen::Server, public ucanopen::SdoSubscriber {
public:
    Server(std::shared_ptr<can::Socket> socket, ucanopen::NodeId node_id, const std::string& name);
protected:
    virtual ucanopen::FrameHandlingStatus handle_sdo(ucanopen::ODEntryIter entry,
                                                     ucanopen::SdoType sdo_type,
                                                     ucanopen::ExpeditedSdoData data) override final;
private:
    uint32_t _errors = 0;
    uint16_t _warnings = 0;

    bool _run_enabled = false;
    bool _emergency_enabled = false;
    float _torque_perunit_ref = 0;
    float _speed_rpm_ref = 0;
public:
    uint32_t errors() const { return _errors; }
    uint16_t warnings() const { return _warnings; }

    void set_run_enabled(bool enabled) { _run_enabled = enabled; }
    void set_emergency_enabled(bool enabled) { _emergency_enabled = enabled; }
    void set_torque(float value_perunit) { _torque_perunit_ref = std::clamp(value_perunit, -1.0f, 1.0f); }
    void set_speed(float value_rpm) { _speed_rpm_ref = value_rpm; }
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

