/**
 * @file gearselector.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "purecan/purecan_def.h"
#include "../atv_def.h"
#include <cstdint>
#include <cstring>
#include <map>
#include <iostream>


namespace atv {


class GearSelector
{
public:
	enum class Gear
	{
		Parking = 0,
		Reverse = 2,
		Neutral = 3,
		Drive = 4
	};

	enum class CarStatus
	{
		Off = 4,
		On = 2//8 - no sense
	};

	enum class SteeringWheelStatus
	{
		Off = 0x80,
		On = 0x00//10
	};

	const std::map<Gear, std::string_view> gearNames = {
		{Gear::Parking, "Parking"},
		{Gear::Reverse, "Reverse"},
		{Gear::Neutral, "Neutral"},
		{Gear::Drive, "Drive"}
	};

private:
	struct Message0x11A
	{
		uint8_t _reserved_byte0 : 4;
		uint8_t gear : 4;
		
		uint8_t _reserved_byte1 : 4;
		uint8_t ecoSelected : 1;
		uint8_t statusCarOnOff : 3;

		uint8_t steeringWheelButton : 8;

		uint8_t heartbeat : 8;

		uint8_t _reserved_byte4 : 6;
		uint8_t clock : 2;

		uint8_t _reserved_byte5 : 8;

		uint8_t multiplexor : 8;

		uint8_t crc : 8;
	};

	Gear m_gear{Gear::Parking};
	bool m_isEcoModeEnabled{false};
	CarStatus m_carStatus{CarStatus::Off};
	SteeringWheelStatus m_steeringWheelStatus{SteeringWheelStatus::Off};

public:
	GearSelector()
	{
		static_assert(sizeof(Message0x11A) == 8);
	}

	void setGear(Gear gear)
	{
		m_gear = gear;
#ifdef STD_COUT_ENABLED
		std::cout << "[gearselector] " << gearNames.at(gear) << " is selected." << std::endl;
#endif	
	}

	void setEcoMode(bool setting)
	{
		m_isEcoModeEnabled = setting;
#ifdef STD_COUT_ENABLED
		std::cout << "[gearselector] ECO mode is " << (setting ? "enabled." : "disabled.") << std::endl;
#endif
	}

	void setCarStatus(CarStatus status)
	{
		m_carStatus = status;
#ifdef STD_COUT_ENABLED
		std::cout << "[gearselector] Car status: " << (status == CarStatus::Off ? "OFF." : "ON.") << std::endl;
#endif		
	}

	void setSteeringWheelStatus(SteeringWheelStatus status)
	{
		m_steeringWheelStatus = status;
#ifdef STD_COUT_ENABLED
		std::cout << "[gearselector] Steering wheel status: " 
				<< (status == SteeringWheelStatus::Off ? "OFF." : "ON.") << std::endl;
#endif	
	}

	purecan::can_payload_va createMessage0x11A()
	{
		const std::vector<uint8_t> heartbeatValues = {0xAA};//{0x55, 0x55, 0xAA, 0xAA};
		static size_t heartbeatIndex = 0;

		static int clock = 0;

		static uint8_t multiplexor = 0;
		Message0x11A message{};

		if (isDuplicateLogEnabled)
		{
			message._reserved_byte0 = 0xE;
		}

		message.gear = static_cast<uint8_t>(m_gear);
		message.ecoSelected = m_isEcoModeEnabled;
		message.statusCarOnOff = static_cast<uint8_t>(m_carStatus);
		message.steeringWheelButton = static_cast<uint8_t>(m_steeringWheelStatus);
		
		message.heartbeat = heartbeatValues[heartbeatIndex];
		heartbeatIndex = (heartbeatIndex + 1) % heartbeatValues.size();

		if (isDuplicateLogEnabled)
		{
			message.clock = clock; //3
			clock = (clock + 1) % 4;
		}

		message.multiplexor = multiplexor;
		multiplexor = (multiplexor + 1) % 4;
		
		purecan::can_payload_va payload(sizeof(Message0x11A));	
		memcpy(payload.data(), &message, sizeof(Message0x11A));

		uint8_t crc = calculateCrc(payload.data(), 7);
		message.crc = crc;
		payload[7] = crc;

		return payload;
	}
};


} // namespace atv

