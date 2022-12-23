/**
 * @file srmdrive_controller.h
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
#include "../server/srmdrive_def.h"


namespace srmdrive {


class Controller
{
private:
	ucanopen::IServer* _driveServer;

	bool _isRunEnabled = false;
	bool _isEmergencyEnabled = false;

	float _torquePuRef = 0;
	float _speedRef = 0;

public:
	Controller(ucanopen::IServer* driveServer);
	
	void powerUp();
	void powerDown();

	void setRunEnabled(bool isEnabled);
	void setEmergencyEnabled(bool isEnabled);

	void setTorque(double valPercents);
	void setSpeed(double val);

	ucanopen::can_payload makeTpdo1();
	ucanopen::can_payload makeTpdo2();
};


} // namespace srmdrive


