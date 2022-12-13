/**
 * @file ucanopen.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen/client/ucanopen_client.h"


namespace global {
extern std::shared_ptr<ucanopen::Client> ucanClient;
}


extern "C" {


///
///
///
unsigned int ucanopen_client_get_nodeid()
{
	return global::ucanClient->nodeId().value();
}


///
///
///
void ucanopen_client_set_nodeid(unsigned int nodeId)
{
	global::ucanClient->setNodeId(ucanopen::NodeId(nodeId));
}


///
///
///
void ucanopen_client_set_serverid(const char* name ,unsigned int nodeId)
{
	global::ucanClient->setServerNodeId(name, ucanopen::NodeId(nodeId));
}


///
///
///
void ucanopen_client_set_tpdo_enabled(bool isEnabled)
{
	isEnabled ? global::ucanClient->enableTpdo() : global::ucanClient->disableTpdo();
}


///
///
///
void ucanopen_client_set_watch_enabled(bool isEnabled)
{
	isEnabled ? global::ucanClient->enableServerWatch() : global::ucanClient->disableServerWatch();
}


///
///
///
void ucanopen_client_set_watch_period(int period)
{
	if (period <= 0) return;

	global::ucanClient->setServerWatchPeriod(std::chrono::milliseconds(period));
}


}


