/**
 * @file atv_vcm.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "atv/vcm/vcm.h"


extern "C" {


void atv_vcm_set_torque(double val)
{
	atv::VehicleControlModule::instance().setTorqueRef(val);
}


}


