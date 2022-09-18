/**
 * @file c_motordrive_controller.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


extern "C" {


void motordrive_controller_set_power_state(bool state);
void motordrive_controller_set_run_state(bool state);
void motordrive_controller_set_emergency_state(bool state);

}


