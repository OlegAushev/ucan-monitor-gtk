/**
 * @file c_motordrive_controller.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include "motordrive/controller/motordrive_controller.h"


extern std::shared_ptr<motordrive::Controller> g_motordriveController;


extern "C" {


///
///
///
void motordrive_controller_set_power_enabled(bool isEnabled)
{
	if (isEnabled)
	{
		g_motordriveController->powerUp();
	}
	else
	{
		g_motordriveController->powerDown();
	}
}


///
///
///
void motordrive_controller_set_run_enabled(bool isEnabled)
{
	g_motordriveController->setRunEnabled(isEnabled);
}


///
///
///
void motordrive_controller_set_emergency_enabled(bool isEnabled)
{
	g_motordriveController->setEmergencyEnabled(isEnabled);
}


///
///
///
void motordrive_controller_set_torque(double valPu)
{
	g_motordriveController->setTorque(valPu);
}


///
///
///
void motordrive_controller_set_speed(double val)
{
	g_motordriveController->setSpeed(val);
}


}


