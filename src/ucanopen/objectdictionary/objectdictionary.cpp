/**
 * @file objectdictionary.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "objectdictionary.h"


namespace ucanopen {


extern const std::map<OD_EntryKey, OD_EntryValue> OBJECT_DICTIONARY = {	
{{0x1008, 0x00}, {"SYSTEM", "INFO", "DEVICE_NAME", "", OD_STRING, OD_ACCESS_RO}},
{{0x5FFF, 0x00}, {"SYSTEM", "INFO", "SOFTWARE_VERSION", "", OD_UINT32, OD_ACCESS_RO}},
{{0x5FFF, 0x01}, {"SYSTEM", "INFO", "BUILD_CONFIGURATION", "", OD_STRING, OD_ACCESS_RO}},

{{0x2000, 0x00}, {"SYSTEM", "SYSLOG", "SYSLOG_MSG", "", OD_UINT32, OD_ACCESS_RO}},


{{0x5000, 0x00}, {"WATCH", "WATCH", "UPTIME",		"s",	OD_FLOAT32,	OD_ACCESS_RO}},
{{0x5000, 0x01}, {"WATCH", "WATCH", "DRIVE_STATE",	"",	OD_ENUM16,	OD_ACCESS_RO}},
{{0x5000, 0x02}, {"WATCH", "WATCH", "VOLTAGE_DC",	"V",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x03}, {"WATCH", "WATCH", "CURRENT_PHU",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x04}, {"WATCH", "WATCH", "CURRENT_PHV",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x05}, {"WATCH", "WATCH", "CURRENT_PHW",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x06}, {"WATCH", "WATCH", "CURRENT_PHX",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x07}, {"WATCH", "WATCH", "CURRENT_PHY",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x08}, {"WATCH", "WATCH", "CURRENT_PHZ",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x09}, {"WATCH", "WATCH", "TEMP_PHU",		"°C",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x0A}, {"WATCH", "WATCH", "TEMP_PHV",		"°C",	OD_FLOAT32,	OD_ACCESS_RO}},
{{0x5000, 0x0B}, {"WATCH", "WATCH", "TEMP_PHW",		"°C",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x0C}, {"WATCH", "WATCH", "TEMP_PHX",		"°C",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x0D}, {"WATCH", "WATCH", "TEMP_PHY",		"°C",	OD_FLOAT32,	OD_ACCESS_RO}},
{{0x5000, 0x0E}, {"WATCH", "WATCH", "TEMP_PHZ",		"°C",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x0F}, {"WATCH", "WATCH", "TEMP_CASE",	"°C",	OD_FLOAT32, 	OD_ACCESS_RO}},

{{0x5000, 0x10}, {"WATCH", "WATCH", "CURRENT_S",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x11}, {"WATCH", "WATCH", "CURRENT_D",	"A",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x5000, 0x12}, {"WATCH", "WATCH", "CURRENT_Q",	"A",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x13}, {"WATCH", "WATCH", "SPEED_RPM",	"rpm",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x5000, 0x14}, {"WATCH", "WATCH", "TORQUE",		"Nm",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x5000, 0x15}, {"WATCH", "WATCH", "POWER_MECH",	"W",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x16}, {"WATCH", "WATCH", "ROTOR ANGLE", 	"°",	OD_FLOAT32, 	OD_ACCESS_WO}},
{{0x5000, 0x17}, {"WATCH", "WATCH", "VOLTAGE_D",	"V",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x18}, {"WATCH", "WATCH", "VOLTAGE_Q",	"V",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x19}, {"WATCH", "WATCH", "FLUX",	 	"Wb",	OD_FLOAT32, 	OD_ACCESS_RO}},
{{0x5000, 0x1A}, {"WATCH", "WATCH", "POWER_ELEC",	"VA",	OD_FLOAT32, 	OD_ACCESS_RO}},


{{0x2001, 0x00}, {"DRIVE", "CONTROL", "POWERUP", 	"",	OD_TASK,	OD_ACCESS_RO,}},
{{0x2001, 0x01}, {"DRIVE", "CONTROL", "POWERDOWN",	"",	OD_TASK, 	OD_ACCESS_RO,}},


{{0x2002, 0x00}, {"SYSTEM", "CONTROL", "RESET_DEVICE", 		"",	OD_TASK, 	OD_ACCESS_RO,}},
{{0x2002, 0x01}, {"SYSTEM", "CONTROL", "RESET_FAULTS", 		"",	OD_TASK, 	OD_ACCESS_RO,}},
{{0x2002, 0x02}, {"SYSTEM", "CONTROL", "RESET_PARAMETERS", 	"",	OD_TASK, 	OD_ACCESS_RO,}},
{{0x2002, 0x03}, {"SYSTEM", "CONTROL", "APPLY_PARAMETERS", 	"",	OD_TASK, 	OD_ACCESS_RO,}},


{{0x2100, 0x00}, {"CONFIG",	"MOTOR",	"PARAMS_KNOWN",	"",	OD_BOOL,	OD_ACCESS_RW}},
{{0x2100, 0x01}, {"CONFIG",	"MOTOR",	"POLE_PAIRS",	"",	OD_INT16, 	OD_ACCESS_RW}},
{{0x2100, 0x02}, {"CONFIG",	"MOTOR",	"Rs",	"Ω",		OD_FLOAT32,	OD_ACCESS_RW}},
{{0x2100, 0x03}, {"CONFIG",	"MOTOR",	"Rr",	"Ω",		OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2100, 0x04}, {"CONFIG",	"MOTOR",	"Lm",	"H",		OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2100, 0x05}, {"CONFIG",	"MOTOR",	"Lls",	"H",		OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2100, 0x06}, {"CONFIG",	"MOTOR",	"Llr",	"H",		OD_FLOAT32, 	OD_ACCESS_RW}},

{{0x2101, 0x00}, {"CONFIG",	"MODEL",	"CONTROL_LOOP_TYPE", 	"o-c",	OD_ENUM16, 	OD_ACCESS_RW}},
{{0x2101, 0x01}, {"CONFIG",	"MODEL",	"REFERENCE", 		"n-M",	OD_ENUM16, 	OD_ACCESS_RW}},
{{0x2101, 0x02}, {"CONFIG",	"MODEL",	"FORWARD_SIGN",		"",	OD_INT16, 	OD_ACCESS_RW}},
{{0x2101, 0x03}, {"CONFIG",	"MODEL",	"KP_SPEED", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x04}, {"CONFIG",	"MODEL",	"KI_SPEED", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x05}, {"CONFIG",	"MODEL",	"KP_ID", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x06}, {"CONFIG",	"MODEL",	"KI_ID", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x07}, {"CONFIG",	"MODEL",	"KP_IQ", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x08}, {"CONFIG",	"MODEL",	"KI_IQ", 		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x09}, {"CONFIG",	"MODEL",	"IS_MOTOR_MAX", 	"A",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0A}, {"CONFIG",	"MODEL",	"IS_GENER_MAX", 	"A",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0B}, {"CONFIG",	"MODEL",	"TORQUE_POS_MAX", 	"Nm",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0C}, {"CONFIG",	"MODEL",	"TORQUE_NEG_MAX",	"Nm",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0D}, {"CONFIG",	"MODEL",	"SPEED_MAX",		"rpm",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0E}, {"CONFIG",	"MODEL",	"KP_FLUXWEAK",		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x0F}, {"CONFIG",	"MODEL",	"KI_FLUXWEAK",		"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2101, 0x10}, {"CONFIG",	"MODEL",	"ID_MAX_FLUXWEAK",	"A",	OD_FLOAT32, 	OD_ACCESS_RW}},

{{0x2102, 0x00}, {"CONFIG",	"CONVERTER",	"UVP_DC",		"V",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x01}, {"CONFIG",	"CONVERTER",	"OVP_DC",		"V",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x02}, {"CONFIG",	"CONVERTER",	"OCP_PHASE", 		"A",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x05}, {"CONFIG",	"CONVERTER",	"OTP_JUNCTION",		"°C",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x06}, {"CONFIG",	"CONVERTER",	"OTP_CASE",		"°C",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x07}, {"CONFIG",	"CONVERTER",	"FAN_TEMP_TH_ON",	"°C",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2102, 0x08}, {"CONFIG",	"CONVERTER",	"FAN_TEMP_TH_OFF",	"°C",	OD_FLOAT32, 	OD_ACCESS_RW}},

{{0x2103, 0x00}, {"CONFIG",	"MRAS",		"KP",			"",	OD_FLOAT32, 	OD_ACCESS_RW}},
{{0x2103, 0x01}, {"CONFIG",	"MRAS",		"KI",			"",	OD_FLOAT32, 	OD_ACCESS_RW}},

{{0x2104, 0x00}, {"CONFIG",	"MCOSERVER",	"PERIOD_HB",		"ms",	OD_UINT32, 	OD_ACCESS_RW}},
{{0x2104, 0x01}, {"CONFIG",	"MCOSERVER",	"PERIOD_TPDO1",		"ms",	OD_UINT32, 	OD_ACCESS_RW}},
{{0x2104, 0x02}, {"CONFIG",	"MCOSERVER",	"PERIOD_TPDO2",		"ms",	OD_UINT32, 	OD_ACCESS_RW}},
{{0x2104, 0x03}, {"CONFIG",	"MCOSERVER",	"PERIOD_TPDO3",		"ms",	OD_UINT32, 	OD_ACCESS_RW}},
{{0x2104, 0x04}, {"CONFIG",	"MCOSERVER",	"PERIOD_TPDO4",		"ms",	OD_UINT32, 	OD_ACCESS_RW}},
};


} // namespace ucanopen

