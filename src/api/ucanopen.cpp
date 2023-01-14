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
#include "logger/logger.h"
#include <cassert>


namespace global {
extern std::shared_ptr<ucanopen::Client> ucanClient;
}


extern "C" {


///
///
///
unsigned int ucanopen_client_get_node_id()
{
	return global::ucanClient->nodeId().value();
}


///
///
///
void ucanopen_client_set_node_id(unsigned int nodeId)
{
	global::ucanClient->setNodeId(ucanopen::NodeId(nodeId));
}


///
///
///
void ucanopen_client_set_server_id(const char* serverName ,unsigned int nodeId)
{
	global::ucanClient->setServerNodeId(serverName, ucanopen::NodeId(nodeId));
}


///
///
///
void ucanopen_client_set_sync_enabled(bool isEnabled)
{
	isEnabled ? global::ucanClient->enableSync() : global::ucanClient->disableSync();
}


///
///
///
void ucanopen_client_set_sync_period(int period)
{
	if (period <= 0) return;
	global::ucanClient->setSyncPeriod(std::chrono::milliseconds(period));
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
void ucanopen_server_get_watch_value(const char* serverName, const char* watchName, char* buf, size_t len)
{
	global::ucanClient->server(serverName)->watchService.value(watchName, buf, len);
}


///
///
///
size_t ucanopen_server_get_config_categories(const char* serverName, char** buf, size_t countMax, size_t lenMax)
{
	size_t ret = global::ucanClient->server(serverName)->configService.entriesList().size();
	if (ret >= countMax)
	{
		return 0;
	}

	size_t i = 0;
	for (auto [category, names] : global::ucanClient->server(serverName)->configService.entriesList())
	{
		strncpy(buf[i++], category.data(), lenMax);
	}

	return ret;
}


///
///
///
size_t ucanopen_server_get_config_entries(const char* serverName, const char* category, char** buf, size_t countMax, size_t lenMax)
{
	auto entries = global::ucanClient->server(serverName)->configService.entriesList().at(category);
	size_t ret = entries.size();
	if (ret >= countMax)
	{
		return 0;
	}
	
	size_t i = 0;
	for (auto entry : entries)
	{
		strncpy(buf[i++], entry.data(), lenMax);
	}

	return ret;
}


///
///
///
bool ucanopen_server_is_heartbeat_ok(const char* serverName)
{
	return global::ucanClient->server(serverName)->heartbeatService.isOk();
}


///
///
///
void ucanopen_server_get_nmt_state(const char* serverName, char* buf, size_t len)
{
	switch (global::ucanClient->server(serverName)->heartbeatService.nmtState())
	{
	case ucanopen::NmtState::Initialization:
		strncpy(buf, "init", len);
		break;
	case ucanopen::NmtState::Stopped:
		strncpy(buf, "stopped", len);
		break;
	case ucanopen::NmtState::Operational:
		strncpy(buf, "run", len);
		break;
	case ucanopen::NmtState::PreOperational:
		strncpy(buf, "pre-run", len);
		break;
	}
}


///
///
///
bool ucanopen_server_is_tpdo_ok(const char* serverName, int tpdoNum)
{
	assert((tpdoNum >= 0) && (tpdoNum <= 3));
	return global::ucanClient->server(serverName)->tpdoService.isOk(static_cast<ucanopen::TpdoType>(tpdoNum));
}


///
///
///
unsigned long ucanopen_server_get_tpdo_data(const char* serverName, int tpdoNum)
{
	assert((tpdoNum >= 0) && (tpdoNum <= 3));
	return ucanopen::fromPayload<uint64_t>(global::ucanClient->server(serverName)->tpdoService.data(static_cast<ucanopen::TpdoType>(tpdoNum)));
}


///
///
///
void ucanopen_server_read(const char* serverName, const char* category, const char* subcategory, const char* name)
{
	global::ucanClient->server(serverName)->read(category, subcategory, name);
}


///
///
///
void ucanopen_server_write(const char* serverName, const char* category, const char* subcategory, const char* name, const char* value)
{
	std::stringstream sstr;
		sstr << "[" << category << "/write] " << subcategory << "::" << name << " = " << value << ", updating...";
	Logger::instance().add(sstr.str());
	global::ucanClient->server(serverName)->write(category, subcategory, name, std::string(value));
}


///
///
///
void ucanopen_server_exec(const char* serverName, const char* category, const char* subcategory, const char* name)
{
	global::ucanClient->server(serverName)->exec(category, subcategory, name);
}


}


