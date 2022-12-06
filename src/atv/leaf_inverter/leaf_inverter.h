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
		uint8_t voltageDC : 8;

		uint8_t _reserved_byte1 : 8;

		uint8_t torqueEffectiveM : 3;
		uint8_t _reserved_byte2 : 5;

		uint8_t torqueEffectiveL : 8;

		uint8_t outputRevolutionM : 8;

		uint8_t _reserved_byte5 : 1;
		uint8_t outputRevolutionL : 7;

		uint8_t clock : 2;
		uint8_t _reserved_byte6 : 2;
		uint8_t error : 4;

		uint8_t crc;
	};

	struct Message0x55A
	{
		uint8_t _reserved_byte0 : 8;

		uint8_t tempInverterComBoard;

		uint8_t tempIgbt;

		uint8_t tempIgbtDriver;

		uint8_t tempMotor;

		uint8_t _reserved_byte5 : 8;

		uint8_t _reserved_byte6 : 8;

		uint8_t _reserved_byte7 : 8;	
	};

	double m_voltageDC{0};
	double m_torqueEffective{0};
	double m_outputRevolution{0};
	bool m_hasError{false};

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
		
		auto handlerMessage0x1DA = [this](purecan::can_payload data) { this->handleMessage0x1DA(data); };
		registerTxMessage(0x1DA, "Inverter Response", std::chrono::milliseconds(500), handlerMessage0x1DA);

		auto handlerMessage0x55A = [this](purecan::can_payload data) { this->handleMessage0x55A(data); };
		registerTxMessage(0x55A, "Inverter Temperatures", std::chrono::milliseconds(500), handlerMessage0x55A);
	}

	void handleMessage0x1DA(purecan::can_payload data)
	{
		Message0x1DA message{};
		memcpy(&message, data.data(), 8);

		m_voltageDC = message.voltageDC * 2.0;
		uint16_t torqueEffective = (message.torqueEffectiveM << 8) + message.torqueEffectiveL;
		m_torqueEffective = torqueEffective * 0.5 - 300;
		m_hasError = message.error & 0xB;
	}

	void handleMessage0x55A(purecan::can_payload data)
	{
		Message0x55A message{};
		memcpy(&message, data.data(), 8);

		m_tempInverterComBoard = message.tempInverterComBoard * 0.5;
		m_tempIgbt = message.tempIgbt * 0.5;
		m_tempIgbtDriver = message.tempIgbtDriver * 0.5;
		m_tempMotor = message.tempMotor * 0.5;
	}

	double voltageDC() const { return m_voltageDC; }
	double torqueEffective() const { return m_torqueEffective; }
	bool hasError() const { return m_hasError; }
	double tempInverterComBoard() const { return m_tempInverterComBoard; }
	double tempIgbt() const { return m_tempIgbt; }
	double tempIgbtDriver() const { return m_tempIgbtDriver; }
	double tempMotor() const { return m_tempMotor; }
};


}


