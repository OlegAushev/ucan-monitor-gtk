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
}


///
///
///
void Controller::powerDown()
{
	m_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).exec("DRIVE", "CONTROL", "POWERDOWN");
}




}


