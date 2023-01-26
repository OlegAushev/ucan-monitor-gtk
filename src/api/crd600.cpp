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

