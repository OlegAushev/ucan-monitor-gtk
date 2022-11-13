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
#include "ucanopen/server/ucanopen_server.h"
#include "../server/srmdrive_tpdodef.h"


namespace srmdrive {


class Controller
{
private:
	ucanopen::IServer* m_driveServer;

	bool m_isRunEnabled = false;
	bool m_isEmergencyEnabled = false;

	float m_torquePuRef = 0;
	float m_speedRef = 0;

public:
	Controller(ucanopen::IServer* driveServer);
	
	void powerUp();
	void powerDown();

	void setRunEnabled(bool isEnabled);
	void setEmergencyEnabled(bool isEnabled);

	void setTorque(double valPercents);
	void setSpeed(double val);

	std::array<uint8_t, 8> makeTpdo1();
	std::array<uint8_t, 8> makeTpdo2();
};


} // namespace srmdrive


