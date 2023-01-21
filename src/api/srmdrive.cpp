#include <iostream>
#include <cassert>
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"


namespace global {
extern std::shared_ptr<srmdrive::Server> srmdrive_server;
}


extern "C" {


void srmdrive_controller_set_power_enabled(bool is_enabled)
{
	is_enabled ? global::srmdrive_server->controller.powerup() : global::srmdrive_server->controller.powerdown();
}


void srmdrive_controller_set_run_enabled(bool is_enabled)
{
	global::srmdrive_server->controller.set_run(is_enabled);
}


void srmdrive_controller_set_emergency_enabled(bool is_enabled)
{
	global::srmdrive_server->controller.set_emergency(is_enabled);
}


void srmdrive_controller_set_torque(double val_pu)
{
	global::srmdrive_server->controller.set_torque(val_pu);
}


void srmdrive_controller_set_speed(double val)
{
	global::srmdrive_server->controller.set_speed(val);
}

}

