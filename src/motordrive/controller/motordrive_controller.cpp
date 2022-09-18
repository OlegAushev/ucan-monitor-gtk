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


}


