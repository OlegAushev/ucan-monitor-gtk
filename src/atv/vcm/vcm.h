/**
 * @file vcm.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "purecan/purecan_def.h"


class VehicleControlModule
{
private:
	struct Message0x1D4
	{
		uint8_t byte0_reserved : 8;
		uint8_t byte1_reserved : 8;
		uint8_t torqueRequestM : 8;
		uint16_t byte3_reserved : 4;
		uint8_t torqueRequestL : 4;
	};
		



public:




}