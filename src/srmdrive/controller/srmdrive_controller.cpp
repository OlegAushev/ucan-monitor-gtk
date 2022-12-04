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
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] Power UP motor drive." << std::endl;
#endif	
}


///
///
///
void Controller::powerDown()
{
	m_driveServer->exec("DRIVE", "CONTROL", "POWERDOWN");
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] Power DOWN motor drive." << std::endl;
#endif
}


///
///
///
void Controller::setRunEnabled(bool isEnabled)
{
	m_isRunEnabled = isEnabled;
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] RUN state: " << isEnabled << std::endl;
#endif	
}


///
///
///
void Controller::setEmergencyEnabled(bool isEnabled)
{
	m_isEmergencyEnabled = isEnabled;
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] EMERGENCY state: " << isEnabled << std::endl;
#endif		
}


///
///
///
void Controller::setTorque(double valPu)
{
	m_torquePuRef = std::clamp(valPu, -1.0, 1.0);
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] Torque reference: " << m_torquePuRef * 100.0 << "%" << std::endl;
#endif	
}


///
///
///
void Controller::setSpeed(double val)
{
	m_speedRef = val;
#ifdef STD_COUT_ENABLED
	std::cout << "[srmdrive] Speed reference: " << m_speedRef << "rpm" << std::endl;
#endif
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
	
	std::array<uint8_t, 8> ret;
	memcpy(ret.data(), &message, sizeof(CobTpdo1));
	return ret;
}


///
///
///
ucanopen::can_payload Controller::makeTpdo2()
{
	CobTpdo2 message{};

	message.torque = ((m_torquePuRef > 0) ? m_torquePuRef * 32767 : m_torquePuRef * 32768);

	std::array<uint8_t, 8> ret;
	memcpy(ret.data(), &message, sizeof(CobTpdo2));
	return ret;
}


} // namespace srmdrive


