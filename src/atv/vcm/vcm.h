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
#include <iostream>


namespace atv {


class VehicleControlModule
{
public:
	enum class State
	{
		GoToSleep = 0,
		WakeUp = 3
	};

private:
	struct Message0x1D4
	{
		uint8_t _reserved_byte0 : 8;

		uint8_t _reserved_byte1 : 8;
		
		uint8_t torqueRequestM : 8;

		uint8_t _reserved_byte3 : 4;
		uint8_t torqueRequestL : 4;

		uint8_t _reserved_byte4_0 : 2;
		uint8_t statusHvPowerSupply : 1;
		uint8_t _reserved_byte4_1 : 3;
		uint8_t clock : 2;

		uint8_t _reserved_byte5_0 : 6;
		uint8_t statusRelayPlusOutput : 1;
		uint8_t _reserved_byte5_1 : 1;

		uint8_t statusCharge : 8;

		uint8_t crc : 8;
	};

	struct Message0x50B
	{
		uint8_t _reserved_byte0 : 8;
	
		uint8_t _reserved_byte1 : 8;

		uint8_t _reserved_byte2_0 : 2;
		uint8_t diagMuxOn : 1;
		uint8_t _reserved_byte2_1 : 5;

		uint8_t _reserved_byte3 : 6;
		uint8_t cmdWakeUpSleep : 2;

		uint8_t _reserved_byte4 : 8;
		
		uint8_t _reserved_byte5 : 8;
		
		uint8_t _reserved_byte6 : 8;	
	};

	double m_torqueRef{0};
	bool m_hvPowerSupply{false};
	bool m_relayPlusOutput{false};

	State m_state{State::GoToSleep};

private:
	VehicleControlModule()
	{
		static_assert(sizeof(Message0x1D4) == 8);
		static_assert(sizeof(Message0x50B) == 7);
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
#ifdef STD_COUT_ENABLED
		std::cout << "[vcm] HV power supply is " << (state ? "enabled." : "disabled.") << std::endl;
#endif
	}

	void setRelayPlusOutput(bool state)
	{
		m_relayPlusOutput = state;
#ifdef STD_COUT_ENABLED
		std::cout << "[vcm] Relay plus output is " << (state ? "enabled." : "disabled.") << std::endl;
#endif
	}

	void setState(State state)
	{
		m_state = state;
#ifdef STD_COUT_ENABLED
		std::cout << "[vcm] Cmd: " << (state == State::GoToSleep ? "go to sleep." : "wake up.") << std::endl;
#endif
	}

	purecan::can_payload_va createMessage0x1D4()
	{
		static int clock = 0;
		
		Message0x1D4 message{};

		// message._reserved_byte0 = 0xF7;	// TODO as in log
		// message._reserved_byte1 = 0x07;	// TODO as in log
		// message.statusCharge = 0x30; 	// TODO as in log
		// message._reserved_byte4_0 = 0x3;	// TODO as in log

		uint16_t torqueScaled = uint16_t(m_torqueRef * 4.0) << 4;
		uint8_t torque[2];
		memcpy(&torque, &torqueScaled, 2);
		message.torqueRequestM = torque[1];
		message.torqueRequestL = torque[0] >> 4;

		message.statusHvPowerSupply = m_hvPowerSupply;
		message.statusRelayPlusOutput = m_relayPlusOutput;

		message.clock = clock;
		clock = (clock + 1) % 4;

		purecan::can_payload_va payload(sizeof(Message0x1D4));	
		memcpy(payload.data(), &message, sizeof(Message0x1D4));

		uint8_t crc = calculateCrc(payload.data(), 7);
		message.crc = crc;
		payload[7] = crc;

		return payload;
	}

	purecan::can_payload_va createMessage0x50B()
	{
		Message0x50B message{};

		// message._reserved_byte2_0 = 0x02;	// TODO as in log

		message.cmdWakeUpSleep = static_cast<uint8_t>(m_state);
		message.diagMuxOn = 1;
		
		purecan::can_payload_va payload(sizeof(Message0x50B));	
		memcpy(payload.data(), &message, sizeof(Message0x50B));
		return payload;
	}
};


}


