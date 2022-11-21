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


#include "purecan/device/purecan_device.h"


namespace atv {


class GearSelector : public purecan::IDevice
{
public:
	enum class Gear
	{
		Parking = 0,
		Reverse = 2,
		Neutral = 3,
		Drive = 4
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

		uint8_t _reserved_byte4 : 8;

		uint8_t _reserved_byte5 : 8;

		uint8_t multiplexor : 8;

		uint8_t startupDataUnknown : 8;
	};

	Gear m_gear{Gear::Parking};
	bool m_isEcoModeEnabled{false};

public:
	GearSelector(std::shared_ptr<can::Socket> socket)
		: purecan::IDevice(socket)
	{
		static_assert(sizeof(Message0x11A) == 8);
	}

	void setGear(Gear gear) { m_gear = gear; }

	void setEcoMode(bool state) { m_isEcoModeEnabled = state; }

	purecan::can_payload_va createMessage0x11A()
	{
		static uint8_t heartbeat = 0x55;
		static uint8_t multiplexor = 0;
		Message0x11A message{};

		message.gear = static_cast<uint8_t>(m_gear);
		message.ecoSelected = m_isEcoModeEnabled;
		message.steeringWheelButton = 0x10;	// TODO as in log
		message.heartbeat = heartbeat;
		if (heartbeat == 0x55)
		{
			heartbeat = 0xAA;
		}
		else
		{
			heartbeat = 0x55;
		}

		message.multiplexor = multiplexor;
		multiplexor = (multiplexor + 1) % 4;

		purecan::can_payload_va payload(sizeof(Message0x11A));	
		memcpy(payload.data(), &message, sizeof(Message0x11A));
		return payload;
	}
};


} // namespace atv

