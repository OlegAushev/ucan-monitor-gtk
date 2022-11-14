/**
 * @file srmdrive_rpdodef.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>
#include <cstring>


namespace srmdrive {


/* ========================================================================== */
/* =================== APPLICATION-SPECIFIC PART BEGIN ====================== */
/* ========================================================================== */
/**
 * @brief
 */
struct CobRpdo1
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
	CobRpdo1() = default;
	CobRpdo1(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobRpdo1)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobRpdo1));
		return data;
	}
};


/**
 * @brief
 */
struct CobRpdo2
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
	CobRpdo2() = default;
	CobRpdo2(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobRpdo2)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobRpdo2));
		return data;
	}
};


/**
 * @brief
 */
struct CobRpdo3
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
	uint32_t syslogInfo : 32;
	CobRpdo3() = default;
	CobRpdo3(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobRpdo3)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobRpdo3));
		return data;
	}
};


/**
 * @brief
 */
struct CobRpdo4
{
	uint32_t errors : 32;
	uint32_t warnings : 32;
	CobRpdo4() = default;
	CobRpdo4(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobRpdo4)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobRpdo4));
		return data;
	}
};
/* ========================================================================== */
/* =================== APPLICATION-SPECIFIC PART END ======================== */
/* ========================================================================== */


} // namespace srmdrive


