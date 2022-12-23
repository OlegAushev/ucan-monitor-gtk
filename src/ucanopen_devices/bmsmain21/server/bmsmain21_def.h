/**
 * @file bmsmain21_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>


namespace bmsmain21 {


/**
 * @brief
 */
struct CobTpdo1
{
	uint32_t discreteInputs : 8;
	uint32_t current : 16;
	uint32_t tempMin : 8;
	uint32_t tempMax : 8;
	uint32_t chargePercentage : 8;
	uint32_t voltage : 16;
	CobTpdo1() = default;
};


}


