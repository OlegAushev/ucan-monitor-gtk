/**
 * @file c_motordrive_controller.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include "c_motordrive_controller.h"
#include "motordrive/controller/motordrive_controller.h"


extern std::shared_ptr<motordrive::Controller> g_motordriveController;


///
///
///
void motordrive_controller_set_power_state(bool state)
{
	if (state)
	{
		g_motordriveController->powerUp();
	}
	else
	{
		g_motordriveController->powerDown();
	}
}


