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
Controller::Controller(ucanopen::IServer* driveServer)
	: m_driveServer(driveServer)
{}


///
///
///
void Controller::powerUp()
{
	m_driveServer->exec("DRIVE", "CONTROL", "POWERUP");
	std::cout << "[srmdrive] Power UP motor drive." << std::endl;
}


///
///
///
void Controller::powerDown()
{
	m_driveServer->exec("DRIVE", "CONTROL", "POWERDOWN");
	std::cout << "[srmdrive] Power DOWN motor drive." << std::endl;
}


///
///
///
void Controller::setRunEnabled(bool isEnabled)
{
	m_isRunEnabled = isEnabled;
	std::cout << "[srmdrive] RUN state: " << isEnabled << std::endl;
}


///
///
///
void Controller::setEmergencyEnabled(bool isEnabled)
{
	m_isEmergencyEnabled = isEnabled;
	std::cout << "[srmdrive] EMERGENCY state: " << isEnabled << std::endl;
}


///
///
///
void Controller::setTorque(double valPu)
{
	m_torquePuRef = std::clamp(valPu, -1.0, 1.0);
	std::cout << "[srmdrive] Torque reference: " << m_torquePuRef * 100.0 << "%" << std::endl;
}


///
///
///
void Controller::setSpeed(double val)
{
	m_speedRef = val;
	std::cout << "[srmdrive] Speed reference: " << m_speedRef << "rpm" << std::endl;
	m_driveServer->write("WATCH", "WATCH", "SPEED_RPM", ucanopen::CobSdoData(m_speedRef));
}


///
///
///
ucanopen::can_payload Controller::makeTpdo1()
{
	CobTpdo1 message{};

	message.run = ((m_isRunEnabled) ? 1 : 0);
	message.emergencyStop = ((m_isEmergencyEnabled) ? 1 : 0);

	return ucanopen::toPayload(message);
}


///
///
///
ucanopen::can_payload Controller::makeTpdo2()
{
	CobTpdo2 message{};

	message.torque = ((m_torquePuRef > 0) ? m_torquePuRef * 32767 : m_torquePuRef * 32768);

	return ucanopen::toPayload(message);
}


} // namespace srmdrive


