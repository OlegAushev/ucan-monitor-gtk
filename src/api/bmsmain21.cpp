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


#include "ucanopen_devices/bmsmain21/server/bmsmain21_server.h"


namespace global {
extern std::shared_ptr<bmsmain21::Server> bmsmain21Server;
}


extern "C" {


double bmsmain21_tpdo1_get_voltage()
{
	return global::bmsmain21Server->voltage();
}

double bmsmain21_tpdo1_get_current()
{
	return global::bmsmain21Server->current();
}

double bmsmain21_tpdo1_get_charge()
{
	return global::bmsmain21Server->charge();
}

double bmsmain21_tpdo1_get_temp_max()
{
	return global::bmsmain21Server->tempMax();
}

double bmsmain21_tpdo1_get_temp_min()
{
	return global::bmsmain21Server->tempMin();
}



}


