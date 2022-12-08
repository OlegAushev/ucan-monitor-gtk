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


///
///
///
void srmdrive_observer_get_watch_value(const char* name, char* retval)
{
	global::srmdriveServer->observer.watchValue(name, retval);
}


///
///
///
bool srmdrive_is_connection_ok()
{
	return global::srmdriveServer->isConnectionOk();
}


///
///
///
bool srmdrive_is_tpdo_ok(int tpdoNum)
{
	assert((tpdoNum >= 0) && (tpdoNum <= 3));
	return global::srmdriveServer->isTpdoOk(static_cast<ucanopen::TpdoType>(tpdoNum));
}


}


