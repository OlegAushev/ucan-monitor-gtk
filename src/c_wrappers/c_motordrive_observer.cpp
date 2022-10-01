/**
 * @file c_motordrive_observer.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include "motordrive/observer/motordrive_observer.h"


extern std::shared_ptr<motordrive::Observer> g_motordriveObserver;


extern "C" {


///
///
///
void motordrive_data_get_watch(const char* name, char* retval)
{
	g_motordriveObserver->watchValue(name, retval);
}








}


