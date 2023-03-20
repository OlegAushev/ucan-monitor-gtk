#pragma once


#include <cstdint>
#include <string>
#include <vector>


namespace launchpad {

struct CobTpdo4 {
    uint64_t counter : 2;
    uint64_t errors : 31;
    uint64_t warnings : 31;
};


struct CobRpdo1 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobRpdo2 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobRpdo3 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobRpdo4 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobClientTpdo1 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobClientTpdo2 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobClientTpdo3 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


struct CobClientTpdo4 {
    uint32_t counter : 2;
    uint32_t _reserved : 30;
    float value;
};


inline const std::vector<std::string> syslog_messages = {
    "",
    "         [LaunchPad::syslog] Boot CPU1...",
    "[  OK  ] [LaunchPad::syslog] CPU1 ready.",
    "         [LaunchPad::syslog] Boot CPU2...",
    "[  OK  ] [LaunchPad::syslog] CPU2 ready.",
    "[  OK  ] [LaunchPad::syslog] Configured CPU1 periphery.",
    "[  OK  ] [LaunchPad::syslog] Configured CPU2 periphery.",
    "[  OK  ] [LaunchPad::syslog] Device ready.",
    "[FAILED] [LaunchPad::syslog] Device busy.",
    "         [LaunchPad::syslog] Resetting device...",
    "[ WARN ] [LaunchPad::syslog] Lost SDO request.",
    "[ WARN ] [LaunchPad::syslog] Cannot communicate with EEPROM: disabled.",
    "[  OK  ] [LaunchPad::syslog] Read settings.",
    "[FAILED] [LaunchPad::syslog] Failed to read settings.",
    "[  OK  ] [LaunchPad::syslog] Applied settings.",
    "[FAILED] [LaunchPad::syslog] Failed to apply settings.",
    "[  OK  ] [LaunchPad::syslog] Restored settings.",
    "[FAILED] [LaunchPad::syslog] Failed to restore settings.",
    "[  OK  ] [LaunchPad::syslog] Loaded default settings.",
};


inline const std::vector<std::string_view> error_list = {
    "can_bus_connection_lost",
};


inline const std::vector<std::string_view> warning_list = {
    "can_bus_error",
    "can_bus_overrun",
    "can_bus_connection_lost",
};

} // namespace launchpad

