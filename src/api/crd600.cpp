#include <iostream>
#include <cassert>
#include "ucanopen_devices/crd600/server/crd600_server.h"


std::shared_ptr<crd600::Server> crd600_server;


namespace api {
void register_crd600_server(std::shared_ptr<crd600::Server> crd600_server_)
{
	crd600_server = crd600_server_;
}
}


extern "C" {

void crd600_set_power_enabled(bool is_enabled)
{
	if (is_enabled)
	{
		crd600_server->exec("control", "drive", "powerup");
	}
	else
	{
		crd600_server->exec("control", "drive", "powerdown");
	}
}

void crd600_set_drive1_run_enabled(bool is_enabled)
{
	crd600_server->drive1_run = is_enabled;
}

void crd600_set_drive2_run_enabled(bool is_enabled)
{
	crd600_server->drive2_run = is_enabled;
}

void crd600_set_emergency_enabled(bool is_enabled)
{
	crd600_server->emergency_stop = is_enabled;
}

void crd600_set_drive1_speed_ref(double val)
{
	crd600_server->set_drive1_speed_ref(val);
}

void crd600_set_drive2_speed_ref(double val)
{
	crd600_server->set_drive2_speed_ref(val);
}

void crd600_set_drive1_torque_ref(double val_pu)
{
	crd600_server->set_drive1_torque_ref(val_pu);
}
	
void crd600_set_drive2_torque_ref(double val_pu)
{
	crd600_server->set_drive2_torque_ref(val_pu);
}

////////////////////////////////////////////////////////////////////////////////

void crd600_tpdo1_get_drive1_state(char* buf, size_t len)
{
	strncpy(buf, crd600_server->tpdo1.drive1_state.c_str(), len);
}

void crd600_tpdo1_get_drive2_state(char* buf, size_t len)
{
	strncpy(buf, crd600_server->tpdo1.drive2_state.c_str(), len);
}

void crd600_tpdo1_get_drive1_ref(char* buf, size_t len)
{
	strncpy(buf, crd600_server->tpdo1.drive1_ref.c_str(), len);
}

void crd600_tpdo1_get_drive2_ref(char* buf, size_t len)
{
	strncpy(buf, crd600_server->tpdo1.drive2_ref.c_str(), len);
}

bool crd600_tpdo1_get_drive1_run()
{
	return crd600_server->tpdo1.status_drive1_run;
}

bool crd600_tpdo1_get_drive2_run()
{
	return crd600_server->tpdo1.status_drive2_run;
}

bool crd600_tpdo1_get_error()
{
	return crd600_server->tpdo1.status_error;
}

bool crd600_tpdo1_get_warning()
{
	return crd600_server->tpdo1.status_warning;
}

bool crd600_tpdo1_get_overheat()
{
	return crd600_server->tpdo1.status_overheat;
}

void crd600_tpdo1_get_control_loop_type(char* buf, size_t len)
{
	strncpy(buf, crd600_server->tpdo1.control_loop_type.c_str(), len);
}

}
