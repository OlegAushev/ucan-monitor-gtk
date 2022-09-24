/**
 * @file motordrive_controller.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "motordrive_controller.h"


namespace motordrive {


///
///
///
Controller::Controller(std::shared_ptr<ucanopen::Client> ucanClient)
{
	m_ucanClient = ucanClient;
}


///
///
///
void Controller::powerUp()
{
	m_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).exec("DRIVE", "CONTROL", "POWERUP");
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Power UP motor drive." << std::endl;
#endif	
}


///
///
///
void Controller::powerDown()
{
	m_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).exec("DRIVE", "CONTROL", "POWERDOWN");
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Power DOWN motor drive." << std::endl;
#endif
}


///
///
///
void Controller::setRunState(bool state)
{
	m_runState = state;
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] RUN state: " << state << std::endl;
#endif	
}


///
///
///
void Controller::setEmergencyState(bool state)
{
	m_emergencyState = state;
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] EMERGENCY state: " << state << std::endl;
#endif		
}


///
///
///
void Controller::setTorque(double valPu)
{
	m_torquePuRef = std::clamp(valPu, -1.0, 1.0);
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Torque reference: " << m_torquePuRef * 100.0 << "%" << std::endl;
#endif	
}


///
///
///
void Controller::setSpeed(double val)
{
	m_speedRef = val;
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Speed reference: " << m_speedRef << "rpm" << std::endl;
#endif
}


///
///
///
std::array<uint8_t, 8> Controller::makeTpdo1()
{
	ucanopen::CobTpdo1 message = {};

	message.run = ((m_runState) ? 1 : 0);
	message.emergencyStop = ((m_emergencyState) ? 1 : 0);
	
	std::array<uint8_t, 8> ret;
	memcpy(ret.data(), &message, sizeof(ucanopen::CobTpdo1));
	return ret;
}


///
///
///
std::array<uint8_t, 8> Controller::makeTpdo2()
{
	ucanopen::CobTpdo2 message = {};
	
	//message.speed = m_speedRef;
	//message.torque = m_torquePuRef;

	std::array<uint8_t, 8> ret;
	memcpy(ret.data(), &message, sizeof(ucanopen::CobTpdo2));
	return ret;
}





}


