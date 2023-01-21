/**
 * @file srmdrive_controller.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "srmdrive_controller.h"


namespace srmdrive {


///
///
///
Controller::Controller(ucanopen::Server* driveServer)
	: _driveServer(driveServer)
{}


///
///
///
void Controller::powerUp()
{
	_driveServer->exec("DRIVE", "CONTROL", "POWERUP");
	std::cout << "[srmdrive] Power UP motor drive." << std::endl;
}


///
///
///
void Controller::powerDown()
{
	_driveServer->exec("DRIVE", "CONTROL", "POWERDOWN");
	std::cout << "[srmdrive] Power DOWN motor drive." << std::endl;
}


///
///
///
void Controller::setRunEnabled(bool isEnabled)
{
	_isRunEnabled = isEnabled;
	std::cout << "[srmdrive] RUN state: " << isEnabled << std::endl;
}


///
///
///
void Controller::setEmergencyEnabled(bool isEnabled)
{
	_isEmergencyEnabled = isEnabled;
	std::cout << "[srmdrive] EMERGENCY state: " << isEnabled << std::endl;
}


///
///
///
void Controller::setTorque(double valPu)
{
	_torquePuRef = std::clamp(valPu, -1.0, 1.0);
	std::cout << "[srmdrive] Torque reference: " << _torquePuRef * 100.0 << "%" << std::endl;
}


///
///
///
void Controller::setSpeed(double val)
{
	_speedRef = val;
	std::cout << "[srmdrive] Speed reference: " << _speedRef << "rpm" << std::endl;
	_driveServer->write("WATCH", "WATCH", "SPEED_RPM", ucanopen::CobSdoData(_speedRef));
}


///
///
///
ucanopen::can_payload Controller::makeTpdo1()
{
	CobRpdo1 message{};

	message.run = ((_isRunEnabled) ? 1 : 0);
	message.emergencyStop = ((_isEmergencyEnabled) ? 1 : 0);

	return ucanopen::to_payload(message);
}


///
///
///
ucanopen::can_payload Controller::makeTpdo2()
{
	CobRpdo2 message{};

	message.torque = ((_torquePuRef > 0) ? _torquePuRef * 32767 : _torquePuRef * 32768);

	return ucanopen::to_payload(message);
}


} // namespace srmdrive


