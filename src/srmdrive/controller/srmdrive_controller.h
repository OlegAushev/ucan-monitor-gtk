/**
 * @file motordrive_controller.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <algorithm>
#include "ucanopen/client/ucanopen_client.h"


namespace motordrive {


class Controller
{
private:
	std::shared_ptr<ucanopen::Client> m_ucanClient;

	bool m_isRunEnabled = false;
	bool m_isEmergencyEnabled = false;

	float m_torquePuRef = 0;
	float m_speedRef = 0;

public:
	Controller(std::shared_ptr<ucanopen::Client> ucanClient);
	
	void powerUp();
	void powerDown();

	void setRunEnabled(bool isEnabled);
	void setEmergencyEnabled(bool isEnabled);

	void setTorque(double valPercents);
	void setSpeed(double val);

	std::array<uint8_t, 8> makeTpdo1();
	std::array<uint8_t, 8> makeTpdo2();
};


} // namespace motordrive


