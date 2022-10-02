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
void motordrive_observer_get_watch_value(const char* name, char* retval)
{
	g_motordriveObserver->watchValue(name, retval);
}


///
///
///
void motordrive_observer_set_watch_enabled(bool isEnabled)
{
	g_motordriveObserver->setWatchEnabled(isEnabled);
}






}


