/**
 * @file leaf_inverter.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "purecan/device/purecan_device.h"


namespace atv {


class LeafInverter : public purecan::IDevice
{
private:
	struct Message0x1DA
	{
		uint8_t byte0_reserved : 8;

		uint8_t byte1_reserved : 8;

		uint8_t torqueEffectiveM : 3;
		uint8_t byte2_reserved : 5;

		uint8_t torqueEffectiveL : 8;

		uint8_t outputRevolutionM : 8;

		uint8_t byte5_reserved : 1;
		uint8_t outputRevolutionL : 7;

		uint8_t clock : 2;
		uint8_t byte6_reserved : 6;

		uint8_t crc;
	};

	struct Message0x55A
	{
		uint8_t byte0_reserved : 8;
		uint8_t tempInverterComBoard;
		uint8_t tempIgbt;
		uint8_t tempIgbtDriver;
		uint8_t tempMotor;
		uint8_t byte5_reserved : 8;
		uint8_t byte6_reserved : 8;
		uint8_t byte7_reserved : 8;	
	};

	double m_torqueEffective{0};
	double m_outputRevolution{0};

	double m_tempInverterComBoard{0};
	double m_tempIgbt{0};
	double m_tempIgbtDriver{0};
	double m_tempMotor{0};

public:
	LeafInverter(std::shared_ptr<can::Socket> socket)
		: purecan::IDevice(socket)
	{
		static_assert(sizeof(Message0x1DA) == 8);
		static_assert(sizeof(Message0x55A) == 8);

		auto handlerMessage0x1DA_ = [this](purecan::can_payload data) { this->handlerMessage0x1DA(data); };
		registerTxMessage(0x1DA, "Inverter Response", std::chrono::milliseconds(500), handlerMessage0x1DA_);

		auto handlerMessage0x55A_ = [this](purecan::can_payload data) { this->handlerMessage0x55A(data); };
		registerTxMessage(0x55A, "Inverter Temperatures", std::chrono::milliseconds(500), handlerMessage0x55A_);
	}

	void handlerMessage0x1DA(purecan::can_payload data)
	{
		Message0x1DA message{};
		memcpy(&message, data.data(), 8);

		uint16_t torqueEffective = (message.torqueEffectiveM << 8) + message.torqueEffectiveL;
		m_torqueEffective = torqueEffective * 0.5 - 300;
	}

	void handlerMessage0x55A(purecan::can_payload data)
	{
		Message0x55A message{};
		memcpy(&message, data.data(), 8);

		m_tempInverterComBoard = message.tempInverterComBoard * 0.5;
		m_tempIgbt = message.tempIgbt * 0.5;
		m_tempIgbtDriver = message.tempIgbtDriver * 0.5;
		m_tempMotor = message.tempMotor * 0.5;
	}

	double torqueEffective() const { return m_torqueEffective; }
	double tempInverterComBoard() const { return m_tempInverterComBoard; }
	double tempIgbt() const { return m_tempIgbt; }
	double tempIgbtDriver() const { return m_tempIgbtDriver; }
	double tempMotor() const { return m_tempMotor; }
};


}


