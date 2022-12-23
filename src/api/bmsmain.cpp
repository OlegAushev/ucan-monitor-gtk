/**
 * @file bmsmain21.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"


namespace global {
extern std::shared_ptr<bmsmain::Server> bmsmainServer;
}


extern "C" {


double bmsmain_tpdo1_get_voltage()
{
	return global::bmsmainServer->voltage();
}

double bmsmain_tpdo1_get_current()
{
	return global::bmsmainServer->current();
}

double bmsmain_tpdo1_get_charge()
{
	return global::bmsmainServer->charge();
}

double bmsmain_tpdo1_get_temp_max()
{
	return global::bmsmainServer->tempMax();
}

double bmsmain_tpdo1_get_temp_min()
{
	return global::bmsmainServer->tempMin();
}



}


