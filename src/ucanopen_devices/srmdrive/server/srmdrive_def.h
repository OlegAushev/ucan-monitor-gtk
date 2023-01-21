#pragma once


#include <cstdint>
#include <cstring>
#include <vector>
#include <string_view>
#include <string>


namespace srmdrive {

struct CobTpdo1
{
	uint16_t status_run : 1;	// bit 0
	uint16_t status_fault : 1;	// bit 1
	uint16_t status_warning : 1;	// bit 2
	uint16_t status_overheat : 1;	// bit 3
	uint16_t status_reserved : 4;	// bit 4..7
	int8_t torque : 8;
	int16_t speed : 16;
	int8_t current_s : 8;
	int8_t power : 8;
	uint8_t voltage_dc : 8;
	uint8_t current_f : 8;
	CobTpdo1() = default;
};


struct CobTpdo2
{
	uint8_t temp_motor_s : 8;
	uint8_t _reserved1 : 8;
	uint8_t voltage_out : 8;
	uint8_t _reserved2 : 7;
	uint16_t torque_limitation : 1;
	uint8_t temp_motor_fw : 8;
	uint8_t temp_heatsink : 8;
	uint8_t tepm_case_air : 8;
	uint8_t _reserved3 : 8;
	CobTpdo2() = default;
};


struct CobTpdo3
{
	uint8_t voltage_pos_housing : 8;
	uint8_t voltage_neg_housing : 8;
	uint16_t status_reserved1 : 1;			// bit 0
	uint16_t status_insulation_low : 1;		// bit 1
	uint16_t status_insulation_low_wofilter : 1;	// bit 2
	uint16_t status_reserved2 : 1;			// bit 3
	uint16_t _reserved1 : 3;
	uint16_t drive_reference : 1;
	int8_t current_dc : 8;
	uint32_t syslog_message_id : 32;
	CobTpdo3() = default;
};


struct CobTpdo4
{
	uint32_t errors : 32;
	uint16_t _reserved : 16;
	uint16_t warnings : 16;
	CobTpdo4() = default;
};


struct CobRpdo1
{
	uint16_t run : 1;
	uint32_t _reserved1 : 31;
	uint16_t emergency_stop : 1;
	uint16_t _reserved2 : 1;
	uint16_t braking : 1;
	uint32_t _reserved3 : 29;
	CobRpdo1() = default;
};


struct CobRpdo2
{
	int8_t torque_obsolete : 8;
	uint8_t _reserved1 : 8;
	int16_t torque : 16;
	uint32_t _reserved2 : 32;
	CobRpdo2() = default;
};


struct CobRpdo3
{
	uint64_t data;
};


struct CobRpdo4
{
	uint64_t data;
};


inline const std::vector<std::string> syslog_messages = {
	"No message",
	"[syslog] Device boot - success.",
	"[syslog] Device is busy.",
	"[syslog] Reset device...",
	"[syslog] Read configs from EEPROM - success.",
	"[syslog] Read configs from EEPROM - fail.",
	"[syslog] Reset configs - success.",
	"[syslog] Reset configs - fail.",
	"[syslog] Apply configs - success.",
	"[syslog] Apply configs - fail.",
	"[syslog] Position sensor calibration - success.",
	"[syslog] Position sensor calibration - fail.",
	"[syslog] Write calibration data to EEPROM - success.",
	"[syslog] Write calibration data to EEPROM - fail.",
	"[syslog] Operation cannot be performed."	
};


inline const std::vector<std::string> drive_states = {
	"standby",
	"idle",
	"pwrup",
	"ready",
	"prep",
	"start",
	"run",
	"stop",
	"pwrdown",
	"cal pwrup",
	"cal",
	"eval",
} ;


inline const std::vector<std::string_view> error_list = {
	"DC_UNDERVOLTAGE",		// 0x0000 0001
	"DC_OVERVOLTAGE",		// 0x0000 0002
	"PHASE_OVERCURRENT",		// 0x0000 0004
	"FIELD_OVERCURRENT",		// 0x0000 0008
	"DC_OVERCURRENT",		// 0x0000 0010
	"DRIVER_3PH_FLT",		// 0x0000 0020
	"DRIVER_1PH_FLT",		// 0x0000 0040
	"JUNCTION_OVERTEMP",		// 0x0000 0080
	"CASE_OVERTEMP",		// 0x0000 0100
	"STATOR_OVERTEMP",		// 0x0000 0200
	"FIELD_WINDING_OVERTEMP",	// 0x0000 0400
	"CONNECTION_LOST",		// 0x0000 0800
	"CAN_BUS_ERROR",		// 0x0000 1000
	"RUNTIME_ERROR",		// 0x0000 2000
	"EEPROM_ERROR",			// 0x0000 4000
	"PHASE_CURRENT_SENSOR_FAULT",	// 0x0000 8000
	"FIELD_CURRENT_SENSOR_FAULT",	// 0x0001 0000
	"DC_CURRENT_SENSOR_FAULT",	// 0x0002 0000
	"EMERGENCY_STOP",		// 0x0004 0000
	"DCLINK_CHARGE_FAILURE",	// 0x0008 0000
	"POSSENSOR_CALIBRATION_FAULT",	// 0x0010 0000
	"MOTOR_TEMP_SENSOR_FAULT",	// 0x0020 0000
	"BYPASS_CONTACTOR_OPENING_FAULT",	// 0x0040 0000
};


inline const std::vector<std::string_view> warning_list = {
	"DCLINK_DISCONNECTED",
	"DCLINK_CHARGING",
	"DCLINK_HV_AT_OPEN_CONTACTORS",
	"CAN_BUS_WARNING",
	"JUNCTION_OVERHEATING",
	"CASE_OVERHEATING",
	"STATOR_OVERHEATING",
	"FIELD_WINDING_OVERHEATING",
	"FLUX_WEAKENING"
};

}

