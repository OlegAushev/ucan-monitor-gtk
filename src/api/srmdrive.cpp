/**
 * @file c_srmdrive.cpp
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
#include "srmdrive/server/srmdrive_server.h"


extern std::shared_ptr<srmdrive::Server> g_srmdriveServer;


extern "C" {


///
///
///
void srmdrive_controller_set_power_enabled(bool isEnabled)
{
	if (isEnabled)
	{
		g_srmdriveServer->controller.powerUp();
	}
	else
	{
		g_srmdriveServer->controller.powerDown();
	}
}


///
///
///
void srmdrive_controller_set_run_enabled(bool isEnabled)
{
	g_srmdriveServer->controller.setRunEnabled(isEnabled);
}


///
///
///
void srmdrive_controller_set_emergency_enabled(bool isEnabled)
{
	g_srmdriveServer->controller.setEmergencyEnabled(isEnabled);
}


///
///
///
void srmdrive_controller_set_torque(double valPu)
{
	g_srmdriveServer->controller.setTorque(valPu);
}


///
///
///
void srmdrive_controller_set_speed(double val)
{
	g_srmdriveServer->controller.setSpeed(val);
}


///
///
///
void srmdrive_observer_get_watch_value(const char* name, char* retval)
{
	g_srmdriveServer->observer.watchValue(name, retval);
}


///
///
///
void srmdrive_observer_set_watch_enabled(bool isEnabled)
{
	g_srmdriveServer->observer.setWatchEnabled(isEnabled);
}


///
///
///
void srmdrive_observer_set_watch_period(int period)
{
	if (period <= 0) return;

	g_srmdriveServer->observer.setWatchPeriod(std::chrono::milliseconds(period));
}


}


///
///
///
bool srmdrive_is_connection_ok()
{
	return g_srmdriveServer->isConnectionOk();
}


///
///
///
bool srmdrive_is_tpdo_ok(int tpdoNum)
{
	assert((tpdoNum >= 0) && (tpdoNum <= 3));
	return g_srmdriveServer->isTpdoOk(static_cast<ucanopen::TpdoType>(tpdoNum));
}



