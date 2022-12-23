/**
 * @file srmdrive.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include <cassert>
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"


namespace global {
extern std::shared_ptr<srmdrive::Server> srmdriveServer;
}


extern "C" {


///
///
///
void srmdrive_controller_set_power_enabled(bool isEnabled)
{
	isEnabled ? global::srmdriveServer->controller.powerUp() : global::srmdriveServer->controller.powerDown();
}


///
///
///
void srmdrive_controller_set_run_enabled(bool isEnabled)
{
	global::srmdriveServer->controller.setRunEnabled(isEnabled);
}


///
///
///
void srmdrive_controller_set_emergency_enabled(bool isEnabled)
{
	global::srmdriveServer->controller.setEmergencyEnabled(isEnabled);
}


///
///
///
void srmdrive_controller_set_torque(double valPu)
{
	global::srmdriveServer->controller.setTorque(valPu);
}


///
///
///
void srmdrive_controller_set_speed(double val)
{
	global::srmdriveServer->controller.setSpeed(val);
}


}


