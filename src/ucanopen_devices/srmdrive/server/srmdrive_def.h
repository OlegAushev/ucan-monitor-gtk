#pragma once


#include <cstdint>
#include <cstring>
#include <vector>
#include <string_view>
#include <string>


namespace srmdrive {

struct CobTpdo1 {
    uint16_t counter : 2;
    uint16_t status_run : 1;
    uint16_t status_error : 1;
    uint16_t status_warning : 1;
    uint16_t status_overheat : 1;
    uint16_t reference : 1;
    uint16_t control_loop : 1;
    uint16_t drive_state : 8;
    uint16_t dc_voltage : 16;
    int16_t torque : 16;
    int16_t speed : 16;
};


struct CobTpdo2 {
    uint16_t counter : 2;
    uint16_t stator_current : 14;
    uint16_t field_current;
    uint16_t out_voltage : 8;
    uint16_t mech_power : 8;
    uint16_t elec_power : 8;
    uint16_t _reserved : 8;
};


struct CobTpdo3 {
    uint16_t counter : 2;
    uint16_t _reserved : 14;
    uint16_t _reserved2 : 8;
    uint16_t pwrmodule_temp : 8;
    uint16_t excmodule_temp : 8;
    uint16_t pcb_temp : 8;
    uint16_t aw_temp : 8;
    uint16_t fw_temp : 8;
};


struct CobTpdo4 {
    uint64_t counter : 2;
    uint64_t errors : 31;
    uint64_t warnings : 31;
};


//----------------------------------------------------------------------------------------------------------------------
struct CobRpdo1 {
    uint32_t counter : 2;
    uint32_t run : 1;
    uint32_t _reserved1 : 29;
    uint32_t emergency_stop : 1;
    uint32_t _reserved2 : 31;
};


struct CobRpdo2 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    int16_t torque_ref;
    int16_t speed_ref;
};


//----------------------------------------------------------------------------------------------------------------------
inline const std::vector<std::string> syslog_messages = {
    "",
    "         @syslog: Boot CPU1...",
    "[  OK  ] @syslog: CPU1 ready.",
    "         @syslog: Boot CPU2...",
    "[  OK  ] @syslog: CPU2 ready.",
    "[  OK  ] @syslog: Configured CPU1 periphery.",
    "[  OK  ] @syslog: Configured CPU2 periphery.",
    "[  OK  ] @syslog: Device ready.",
    "[FAILED] @syslog: Device busy.",
    "[FAILED] @syslog: Error occurred.",
    "         @syslog: Resetting device...",
    "[ WARN ] @syslog: Lost SDO request.",
    "[ WARN ] @syslog: Cannot communicate with EEPROM: disabled.",
    "[  OK  ] @syslog: Read settings.",
    "[FAILED] @syslog: Failed to read settings.",
    "[  OK  ] @syslog: Applied settings.",
    "[FAILED] @syslog: Failed to apply settings.",
    "[  OK  ] @syslog: Restored settings.",
    "[FAILED] @syslog: Failed to restore settings.",
    "[  OK  ] @syslog: Loaded default settings.",
    "[  OK  ] @syslog: Calibration completed successfully."	
};


inline const std::vector<std::string> drive_states = {
    "idle",
    "wait",
    "standby",
    "startup",
    "ready",
    "prepare",
    "start",
    "run",
    "stop",
    "shutdown",
    "cal_startup",
    "cal_stage1",
    "cal_stage2",
    "cal_stage3",
};


inline const std::vector<std::string_view> error_list = {
    "emergency_stop",
    "can_bus_connection_lost",
    "dclink_charge_failure",
    "dclink_discharge_failure",
    "dc_undervoltage",
    "dc_overvoltage",
    "pwr_module_fault",
    "exc_module_fault",
    "pwr_overcurrent",
    "exc_overcurrent",
    "pwr_overtemp",
    "exc_overtemp",
    "pcb_overtemp",
    "aw_overtemp",
    "fw_overtemp",
    "ia_sensor_fault",
    "if_sensor_fault",
    "eeprom_error",
};


inline const std::vector<std::string_view> warning_list = {
    "can_bus_error",
    "can_bus_overrun",
    "can_bus_connection_lost",
    "dclink_disconnected",
    "dclink_charging",
    "converter_overheat",
    "motor_overheat",
    "flux_weakening",
    "insulation_low",
    "insulation_na",
};

}

