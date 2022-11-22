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
#include "atv/gearselector/gearselector.h"


namespace global {
extern std::shared_ptr<atv::LeafInverter> leafInverter;
extern std::shared_ptr<atv::GearSelector> gearSelector;
}


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

void atv_vcm_set_wakeup_state(bool state)
{
	state ? atv::VehicleControlModule::instance().setState(atv::VehicleControlModule::State::WakeUp)
		: atv::VehicleControlModule::instance().setState(atv::VehicleControlModule::State::GoToSleep);
}

bool atv_leaf_inverter_is_tx_ok(canid_t id)
{
	return global::leafInverter->isTxOk(id);
}

double atv_leaf_inverter_get_torque()
{
	return global::leafInverter->torqueEffective();
}

double atv_leaf_inverter_get_temp_board()
{
	return global::leafInverter->tempInverterComBoard();
}

double atv_leaf_inverter_get_temp_igbt()
{
	return global::leafInverter->tempIgbt();
}

double atv_leaf_inverter_get_temp_igbt_driver()
{
	return global::leafInverter->tempIgbtDriver();
}

double atv_leaf_inverter_get_temp_motor()
{
	return global::leafInverter->tempMotor();
}


void atv_gear_selector_set_gear(unsigned int gear)
{
	switch (gear)
	{
	case 0:
		global::gearSelector->setGear(atv::GearSelector::Gear::Parking);
		break;
	case 1:
		global::gearSelector->setGear(atv::GearSelector::Gear::Reverse);
		break;
	case 2:
		global::gearSelector->setGear(atv::GearSelector::Gear::Neutral);
		break;
	case 3:
		global::gearSelector->setGear(atv::GearSelector::Gear::Drive);
		break;
	default:
		break;
	}
}


void atv_gear_selector_set_ecomode(bool state)
{
	global::gearSelector->setEcoMode(state);
}


void atv_gear_selector_set_car_status(bool state)
{
	state ? global::gearSelector->setCarStatus(atv::GearSelector::CarStatus::On)
			: global::gearSelector->setCarStatus(atv::GearSelector::CarStatus::Off);
}


void atv_gear_selector_set_steering_wheel_status(bool state)
{
	state ? global::gearSelector->setSteeringWheelStatus(atv::GearSelector::SteeringWheelStatus::On)
			: global::gearSelector->setSteeringWheelStatus(atv::GearSelector::SteeringWheelStatus::Off);
}
















}


