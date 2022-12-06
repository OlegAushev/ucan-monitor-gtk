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


#define ANOTHER_CRC_IMPL


namespace atv {


inline bool isDuplicateLogEnabled = false;


#ifndef ANOTHER_CRC_IMPL 


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


#else


static uint8_t calculateCrc(const uint8_t* data, unsigned int len = 7)
{
	uint8_t crc = 0;
	for (int b=0; b<8; b++) {
		for (int i=7; i>=0; i--) {
			uint8_t bit = ((data[b] &(1 << i)) > 0) ? 1 : 0;
			if(crc >= 0x80) crc = (uint8_t)(((crc << 1) + bit) ^ 0x85);
			else            crc = (uint8_t)((crc << 1) + bit);
		}
	}
	return crc;
}


#endif

	
} // namespace atv



