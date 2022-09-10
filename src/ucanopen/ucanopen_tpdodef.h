/**
 * @file ucanopen_tpdodef.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen_def.h"


namespace ucanopen {

/* ========================================================================== */
/* =================== APPLICATION-SPECIFIC PART BEGIN ====================== */
/* ========================================================================== */
struct CobTpdo1
{
	uint16_t run : 1;
	uint32_t reserved1 : 31;
	uint16_t emergencyStop : 1;
	uint32_t reserved2 : 31;
	CobTpdo1() = default;
	explicit CobTpdo1(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobTpdo1)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobTpdo1));
		return data;
	}
};

struct CobTpdo2
{
	float speed;
	float torque;
	CobTpdo2() = default;
	explicit CobTpdo2(uint64_t rawMsg) { memcpy(this, &rawMsg, sizeof(CobTpdo2)); }
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobTpdo2));
		return data;
	}
};

struct CobTpdo3
{
	uint64_t data;
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobTpdo3));
		return data;
	}
};

struct CobTpdo4
{
	uint64_t data;
	uint64_t all() const
	{
		uint64_t data = 0;
		memcpy(&data, this, sizeof(CobTpdo4));
		return data;
	}
};

/* ========================================================================== */
/* =================== APPLICATION-SPECIFIC PART END ======================== */
/* ========================================================================== */

} // namespace ucanopen
