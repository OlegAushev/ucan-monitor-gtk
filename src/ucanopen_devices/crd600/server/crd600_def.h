#pragma once


#include <cstdint>
#include <string>
#include <vector>


namespace crd600 {

struct CobTpdo1 {
    uint16_t counter : 2;
    uint16_t status_drive1_run : 1;
    uint16_t status_drive2_run : 1;
    uint16_t status_error : 1;
    uint16_t status_warning : 1;
    uint16_t status_overheat : 1;
    uint16_t drive1_ref : 1;
    uint16_t drive2_ref : 1;
    uint16_t control_loop_type : 1;
    uint16_t two_motors : 1;
    uint16_t _status_reserved : 5;
    uint16_t drive1_state : 8;
    uint16_t drive2_state : 8;
    uint32_t _reserved : 32;
};


struct CobTpdo2 {
    uint64_t _reserved;
};


struct CobTpdo3 {
    uint64_t _reserved;
};


struct CobTpdo4 {
    uint64_t counter : 2;
    uint64_t errors : 31;
    uint64_t warnings : 31;
};


struct CobRpdo1 {
    uint32_t counter : 2;
    uint32_t drive1_run : 1;
    uint32_t drive2_run : 1;
    uint32_t _reserved1 : 28;
    uint32_t emergency_stop : 1;
    uint32_t _reserved2 : 31;
};


struct CobRpdo2 {
    int16_t drive1_speed_ref;
    int16_t drive1_torque_ref;
    uint32_t counter : 2;
    uint32_t _reserved : 30;
};


struct CobRpdo3 {
    int16_t drive2_speed_ref;
    int16_t drive2_torque_ref;
    uint32_t counter : 2;
    uint32_t _reserved : 30;
};


inline const std::vector<std::string> syslog_messages = {
    "No message",
    "[CRD600::syslog] Boot CPU1...",
    "[CRD600::syslog] Boot CPU1 - success.",
    "[CRD600::syslog] Boot CPU2...",
    "[CRD600::syslog] Boot CPU2 - success.",
    "[CRD600::syslog] CPU1 is ready.",
    "[CRD600::syslog] CPU2 is ready.",
    "[CRD600::syslog] Device is ready.",
    "[CRD600::syslog] Device is busy.",
    "[CRD600::syslog] Resetting device...",
    "[CRD600::syslog] SDO request lost.",
    "[CRD600::syslog] Read configs - success.",	
    "[CRD600::syslog] Read configs - fail.",
    "[CRD600::syslog] Reset configs - success.",
    "[CRD600::syslog] Reset configs - fail.",
    "[CRD600::syslog] Apply configs - success.",
    "[CRD600::syslog] Apply configs - fail.",
};


inline const std::vector<std::string_view> error_list = {
    "dc_undervoltage",
    "dc_overvoltage",
    "phase_overcurrent",
    "driver_fault_all",
    "driver_fault_uvw",
    "driver_fault_xyz",
    "driver_fault_u",
    "driver_fault_v",
    "driver_fault_w",
    "driver_fault_x",
    "driver_fault_y",
    "driver_fault_z",
    "module_overtemp",
    "case_overtemp",
    "eeprom_fault",
    "current_sensor_fault",
    "emergency_stop",
    "can_bus_connection_lost",
};


inline const std::vector<std::string_view> warning_list = {
    "module_overheating",
    "case_overheating",
    "flux_weakening",
    "can_bus_error",
    "can_bus_overrun",
    "can_bus_connection_lost",
};


inline const std::vector<std::string> drive_states = {
    "standby",
    "idle",
    "startup",
    "ready",
    "preparing",
    "start",
    "run",
    "stop",
    "shutdown",
    "eval"
};

} // namespcae crd600

