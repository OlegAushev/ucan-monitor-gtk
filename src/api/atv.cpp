/**
 * @file atv.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "atv/vcm/vcm.h"
#include "atv/leaf_inverter/leaf_inverter.h"


extern std::shared_ptr<atv::LeafInverter> g_leafInverter;


extern "C" {


void atv_vcm_set_torque(double val)
{
	atv::VehicleControlModule::instance().setTorqueRef(val);
}


void atv_vcm_set_hv_power_supply(bool state)
{
	atv::VehicleControlModule::instance().setHvPowerSupply(state);
}


void atv_vcm_set_relay_plus_output(bool state)
{
	atv::VehicleControlModule::instance().setRelayPlusOutput(state);
}


double atv_leaf_inverter_get_torque()
{
	return g_leafInverter->torqueEffective();
}


}


