/**
 * @file atv_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>


namespace atv {


inline uint8_t calculateCrc(const uint8_t* buf, unsigned int len)
{
	const uint8_t poly = 0x85;	// Nissan poly value
	uint8_t crc = 0x00;		// Nissan init value

	while (len--)
	{
		crc ^= *buf++;
		for (auto i = 0; i < 8; i++)
		{
			crc = crc & 0x80 ? (crc << 1) ^ poly : crc << 1;
		}
	}

	return crc;
}

	
} // namespace atv



