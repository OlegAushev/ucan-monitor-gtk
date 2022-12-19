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


///
///
///
void ucanopen_server_get_watch_value(const char* name, const char* watch, char* buf, size_t len)
{
	global::ucanClient->server(name)->watchValue(watch, buf, len);
}


///
///
///
size_t ucanopen_server_get_conf_categories(const char* name, char** buf, size_t size, size_t len)
{
	size_t ret = global::ucanClient->server(name)->confEntriesList().size();
	if (ret >= size)
	{
		return 0;
	}

	size_t i = 0;
	for (auto [category, names] : global::ucanClient->server(name)->confEntriesList())
	{
		strncpy(buf[i++], category.data(), len);
	}

	return ret;
}


///
///
///
size_t ucanopen_server_get_conf_entries(const char* name, const char* category, char** buf, size_t size, size_t len)
{
	auto entries = global::ucanClient->server(name)->confEntriesList().at(category);
	size_t ret = entries.size();
	if (ret >= size)
	{
		return 0;
	}
	
	size_t i = 0;
	for (auto entry : entries)
	{
		strncpy(buf[i++], entry.data(), len);
	}

	return ret;
}


}


