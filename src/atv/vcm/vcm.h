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


#include "../atv_def.h"
#include "purecan/purecan_def.h"
#include <cstring>
#include <algorithm>


namespace atv {


class VehicleControlModule
{
private:
	struct Message0x1D4
	{
		uint8_t byte0_reserved : 8;

		uint8_t byte1_reserved : 8;
		
		uint8_t torqueRequestM : 8;

		uint8_t byte3_reserved : 4;
		uint8_t torqueRequestL : 4;

		uint8_t byte4_reserved0 : 2;
		uint8_t statusHvPowerSupply : 1;
		uint8_t byte4_reserved1 : 3;
		uint8_t clock : 2;

		uint8_t byte5_reserved0 : 6;
		uint8_t statusRelayPlusOutput : 1;
		uint8_t byte5_reserved1 : 1;

		uint8_t statusCharge : 8;

		uint8_t crc : 8;
	};

	double m_torqueRef{0};
	bool m_hvPowerSupply{false};
	bool m_relayPlusOutput{false};

private:
	VehicleControlModule()
	{
		static_assert(sizeof(Message0x1D4) == 8);
	}

public:
	VehicleControlModule(const VehicleControlModule& other) = delete;
	VehicleControlModule& operator=(const VehicleControlModule& other) = delete;

	static VehicleControlModule& instance()
	{
		static VehicleControlModule instance_;
		return instance_;
	}

	void setTorqueRef(double val)
	{
		m_torqueRef = std::clamp(val, 0.0, 1023.75);
	}

	void setHvPowerSupply(bool state)
	{
		m_hvPowerSupply = state;
	}

	void setRelayPlusOutput(bool state)
	{
		m_relayPlusOutput = state;
	}

	purecan::can_payload_va createMessage0x1D4()
	{
		static int clock = 0;
		
		Message0x1D4 message{};

		// message.byte0_reserved = 0xF7; // TODO
		// message.byte1_reserved = 0x07; // TODO
		// message.statusCharge = 0x30; // TODO

		uint16_t torqueScaled = uint16_t(m_torqueRef * 4.0) << 4;
		uint8_t torque[2];
		memcpy(&torque, &torqueScaled, 2);
		message.torqueRequestM = torque[1];
		message.torqueRequestL = torque[0] >> 4;

		message.statusHvPowerSupply = m_hvPowerSupply;
		message.statusRelayPlusOutput = m_relayPlusOutput;

		message.clock = clock;
		clock = (clock + 1) % 4;

		purecan::can_payload_va payload(8);	
		memcpy(payload.data(), &message, 8);

		uint8_t crc = calculateCrc(payload.data(), 7);
		message.crc = crc;
		payload[7] = crc;

		return payload;
	}
};


}


