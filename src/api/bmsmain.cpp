#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"


namespace global {
extern std::shared_ptr<bmsmain::Server> bmsmain_server;
}


extern "C" {

double bmsmain_tpdo1_get_voltage()
{
	return global::bmsmain_server->voltage();
}

double bmsmain_tpdo1_get_current()
{
	return global::bmsmain_server->current();
}

double bmsmain_tpdo1_get_charge()
{
	return global::bmsmain_server->charge();
}

double bmsmain_tpdo1_get_temp_max()
{
	return global::bmsmain_server->temp_max();
}

double bmsmain_tpdo1_get_temp_min()
{
	return global::bmsmain_server->temp_min();
}

}

