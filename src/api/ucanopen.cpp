#include "ucanopen/client/ucanopen_client.h"
#include "log/log.h"
#include <cassert>


std::shared_ptr<ucanopen::Client> ucanopen_client;


namespace api {
void register_ucanopen_client(std::shared_ptr<ucanopen::Client> ucanopen_client_)
{
	ucanopen_client = ucanopen_client_;
}
}


extern "C" {

unsigned int ucanopen_client_get_node_id()
{
	return ucanopen_client->node_id().value();
}


void ucanopen_client_set_node_id(unsigned int node_id)
{
	ucanopen_client->set_node_id(ucanopen::NodeId(node_id));
}


void ucanopen_client_set_server_id(const char* server_name ,unsigned int node_id)
{
	ucanopen_client->set_server_node_id(server_name, ucanopen::NodeId(node_id));
}


void ucanopen_client_set_sync_enabled(bool is_enabled)
{
	is_enabled ? ucanopen_client->enable_sync() : ucanopen_client->disable_sync();
}


void ucanopen_client_set_sync_period(int period)
{
	if (period <= 0) return;
	ucanopen_client->set_sync_period(std::chrono::milliseconds(period));
}


void ucanopen_client_set_tpdo_enabled(bool is_enabled)
{
	is_enabled ? ucanopen_client->enable_tpdo() : ucanopen_client->disable_tpdo();
}


void ucanopen_client_set_server_rpdo_enabled(bool is_enabled)
{
	is_enabled ? ucanopen_client->enable_server_rpdo() : ucanopen_client->disable_server_rpdo();
}


void ucanopen_client_set_watch_enabled(bool is_enabled)
{
	is_enabled ? ucanopen_client->enable_server_watch() : ucanopen_client->disable_server_watch();
}


void ucanopen_client_set_watch_period(int period)
{
	if (period <= 0) return;
	ucanopen_client->set_server_watch_period(std::chrono::milliseconds(period));
}


void ucanopen_server_get_watch_value(const char* server_name, const char* watch_name, char* buf, size_t len)
{
	ucanopen_client->server(server_name)->watch_service.value(watch_name, buf, len);
}


size_t ucanopen_server_get_config_categories(const char* server_name, char** buf, size_t count_max, size_t len_max)
{
	size_t ret = ucanopen_client->server(server_name)->config_service.entries_list().size();
	if (ret >= count_max)
	{
		return 0;
	}

	size_t i = 0;
	for (auto [category, names] : ucanopen_client->server(server_name)->config_service.entries_list())
	{
		strncpy(buf[i++], category.data(), len_max);
	}

	return ret;
}


size_t ucanopen_server_get_config_entries(const char* server_name, const char* category, char** buf, size_t count_max, size_t len_max)
{
	auto entries = ucanopen_client->server(server_name)->config_service.entries_list().at(category);
	size_t ret = entries.size();
	if (ret >= count_max)
	{
		return 0;
	}
	
	size_t i = 0;
	for (auto entry : entries)
	{
		strncpy(buf[i++], entry.data(), len_max);
	}

	return ret;
}


bool ucanopen_server_is_heartbeat_ok(const char* server_name)
{
	return ucanopen_client->server(server_name)->heartbeat_service.is_ok();
}


void ucanopen_server_get_nmt_state(const char* server_name, char* buf, size_t len)
{
	switch (ucanopen_client->server(server_name)->nmt_state())
	{
		case ucanopen::NmtState::initialization:
			strncpy(buf, "init", len);
			break;
		case ucanopen::NmtState::stopped:
			strncpy(buf, "stopped", len);
			break;
		case ucanopen::NmtState::operational:
			strncpy(buf, "run", len);
			break;
		case ucanopen::NmtState::pre_operational:
			strncpy(buf, "pre-run", len);
			break;
	}
}


bool ucanopen_server_is_tpdo_ok(const char* server_name, uint tpdo_num)
{
	assert(tpdo_num <= 3);
	return ucanopen_client->server(server_name)->tpdo_service.is_ok(static_cast<ucanopen::TpdoType>(tpdo_num));
}


unsigned long ucanopen_server_get_tpdo_data(const char* server_name, uint tpdo_num)
{
	assert(tpdo_num <= 3);
	return ucanopen::from_payload<uint64_t>(ucanopen_client->server(server_name)->tpdo_service.data(static_cast<ucanopen::TpdoType>(tpdo_num)));
}


void ucanopen_server_read(const char* server_name, const char* category, const char* subcategory, const char* name)
{
	ucanopen_client->server(server_name)->read(category, subcategory, name);
}


void ucanopen_server_write(const char* server_name, const char* category, const char* subcategory, const char* name, const char* value)
{
	std::stringstream sstr;
		sstr << "[" << category << "/write] " << subcategory << "::" << name << " = " << value << ", updating...";
	Log() << sstr.str();
	ucanopen_client->server(server_name)->write(category, subcategory, name, std::string(value));
}


void ucanopen_server_exec(const char* server_name, const char* category, const char* subcategory, const char* name)
{
	ucanopen_client->server(server_name)->exec(category, subcategory, name);
}

}

