/**
 * @file launchpad.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <iostream>
#include <cassert>
#include "ucanopen_devices/launchpad/server/launchpad_server.h"


namespace global {
extern std::shared_ptr<launchpad::Server> launchpadServer;
}


extern "C" {


///
///
///
void launchpad_set_client_value(unsigned int tpdoNum, double value)
{
	assert((tpdoNum >= 0) && (tpdoNum <= 3));
	global::launchpadServer->setClientValue(static_cast<ucanopen::TpdoType>(tpdoNum), value);
}


///
///
///
void launchpad_set_server_value(unsigned int rpdoNum, double value)
{
	assert((rpdoNum >= 0) && (rpdoNum <= 3));
	global::launchpadServer->setServerValue(static_cast<ucanopen::RpdoType>(rpdoNum), value);
}


}


