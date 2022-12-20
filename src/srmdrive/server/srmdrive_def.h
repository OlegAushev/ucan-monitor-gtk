/**
 * @file srmdrive_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>
#include <cstring>
#include <vector>


namespace srmdrive {

/**
 * @brief
 */
struct CobTpdo1
{
	uint16_t statusRun : 1;		// bit 0
	uint16_t statusFault : 1;	// bit 1
	uint16_t statusWarning : 1;	// bit 2
	uint16_t statusOverheat : 1;	// bit 3
	uint16_t statusReserved : 4;	// bit 4..7
	int8_t torque : 8;
	int16_t speed : 16;
	int8_t currentS : 8;
	int8_t power : 8;
	uint8_t voltageDC : 8;
	uint8_t currentF : 8;
	CobTpdo1() = default;
};


/**
 * @brief
 */
struct CobTpdo2
{
	uint8_t tempMotorS : 8;
	uint8_t reserved1 : 8;
	uint8_t voltageOut : 8;
	uint8_t reserved2 : 7;
	uint16_t torqueLimitation : 1;
	uint8_t tempMotorFw : 8;
	uint8_t tempHeatsink : 8;
	uint8_t tepmCaseAir : 8;
	uint8_t reserved3 : 8;
	CobTpdo2() = default;
};


/**
 * @brief
 */
struct CobTpdo3
{
	uint8_t voltagePosHousing : 8;
	uint8_t voltageNegHousing : 8;
	uint16_t statusReserved1 : 1;			// bit 0
	uint16_t statusInsulationLow : 1;		// bit 1
	uint16_t statusInsulationLowWoFilter : 1;	// bit 2
	uint16_t statusReserved2 : 1;			// bit 3
	uint16_t reserved1 : 3;
	uint16_t driveReference : 1;
	int8_t currentDC : 8;
	uint32_t syslogMessageId : 32;
	CobTpdo3() = default;
};


/**
 * @brief
 */
struct CobTpdo4
{
	uint32_t errors : 32;
	uint32_t warnings : 32;
	CobTpdo4() = default;
};


struct CobRpdo1
{
	uint16_t run : 1;
	uint32_t reserved1 : 31;
	uint16_t emergencyStop : 1;
	uint16_t reserved2 : 1;
	uint16_t braking : 1;
	uint32_t reserved3 : 29;
	CobRpdo1() = default;
};


struct CobRpdo2
{
	int8_t torqueObsolete : 8;
	uint8_t reserved1 : 8;
	int16_t torque : 16;
	uint32_t reserved2 : 32;
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


inline const std::vector<std::string_view> syslogMessages = {
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


}


