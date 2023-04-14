#include "ucanopen/client/client.h"
#include "log/log.h"
#include <cassert>


std::shared_ptr<ucanopen::Client> ucanopen_client;


namespace api {
void register_ucanopen_client(std::shared_ptr<ucanopen::Client> ucanopen_client_) {
    ucanopen_client = ucanopen_client_;
}
}


extern "C" {

unsigned int ucanopen_client_get_node_id() {
    return ucanopen_client->node_id().get();
}


void ucanopen_client_set_node_id(unsigned int node_id) {
    ucanopen_client->set_node_id(ucanopen::NodeId(node_id));
}


void ucanopen_client_set_server_id(const char* server_name ,unsigned int node_id) {
    ucanopen_client->set_server_node_id(server_name, ucanopen::NodeId(node_id));
}


void ucanopen_client_set_sync_enabled(bool enabled) {
    enabled ? ucanopen_client->enable_sync() : ucanopen_client->disable_sync();
}


void ucanopen_client_set_sync_period(int period) {
    if (period <= 0) return;
    ucanopen_client->set_sync_period(std::chrono::milliseconds(period));
}


void ucanopen_client_set_tpdo_enabled(bool enabled) {
    enabled ? ucanopen_client->enable_tpdo() : ucanopen_client->disable_tpdo();
}


void ucanopen_client_set_server_rpdo_enabled(bool enabled) {
    enabled ? ucanopen_client->enable_server_rpdo() : ucanopen_client->disable_server_rpdo();
}


void ucanopen_client_set_watch_enabled(bool enabled) {
    enabled ? ucanopen_client->enable_server_watch() : ucanopen_client->disable_server_watch();
}


void ucanopen_client_set_watch_period(int period) {
    if (period <= 0) return;
    ucanopen_client->set_server_watch_period(std::chrono::milliseconds(period));
}


void ucanopen_server_get_watch_value(const char* server_name, const char* watch_name, char* retbuf, int bufsize) {
    ucanopen_client->server(server_name)->watch_service.value(watch_name, retbuf, bufsize);
}


int ucanopen_server_get_config_categories(const char* server_name, char** retbuf, int str_count, int str_size) {
    int retval = ucanopen_client->server(server_name)->config_service.object_list().size();
    if (retval >= str_count) {
        return 0;
    }

    int i = 0;
    for (auto [category, objects] : ucanopen_client->server(server_name)->config_service.object_list()) {
        retbuf[i][0] = '\0';
        strncat(retbuf[i++], category.data(), str_size);
    }

    return retval;
}


int ucanopen_server_get_config_objects(const char* server_name, const char* category, char** retbuf, int str_count, int str_size) {
    auto objects = ucanopen_client->server(server_name)->config_service.object_list().at(category);
    int retval = objects.size();
    if (retval >= str_count) {
        return 0;
    }
    
    int i = 0;
    for (auto object : objects) {
        retbuf[i][0] = '\0';
        strncat(retbuf[i++], object.data(), str_size);
    }

    return retval;
}


bool ucanopen_server_is_heartbeat_ok(const char* server_name) {
    return ucanopen_client->server(server_name)->heartbeat_service.is_ok();
}


void ucanopen_server_get_nmt_state(const char* server_name, char* retbuf, int bufsize) {
    retbuf[0] = '\0';
    switch (ucanopen_client->server(server_name)->nmt_state()) {
    case ucanopen::NmtState::initialization:
        strncat(retbuf, "init", bufsize-1);
        break;
    case ucanopen::NmtState::stopped:
        strncat(retbuf, "stopped", bufsize-1);
        break;
    case ucanopen::NmtState::operational:
        strncat(retbuf, "run", bufsize-1);
        break;
    case ucanopen::NmtState::pre_operational:
        strncat(retbuf, "pre-run", bufsize-1);
        break;
    }
}


bool ucanopen_server_is_tpdo_ok(const char* server_name, uint tpdo_num) {
    assert(tpdo_num <= 3);
    return ucanopen_client->server(server_name)->tpdo_service.is_ok(static_cast<ucanopen::TpdoType>(tpdo_num));
}


unsigned long ucanopen_server_get_tpdo_data(const char* server_name, uint tpdo_num) {
    assert(tpdo_num <= 3);
    return ucanopen::from_payload<uint64_t>(ucanopen_client->server(server_name)->tpdo_service.data(static_cast<ucanopen::TpdoType>(tpdo_num)));
}


void ucanopen_server_read(const char* server_name, const char* category, const char* subcategory, const char* name) {
    ucanopen_client->server(server_name)->read(category, subcategory, name);
}


void ucanopen_server_write(const char* server_name, const char* category, const char* subcategory, const char* name, const char* value) {
    ucanopen_client->server(server_name)->write(category, subcategory, name, std::string(value));
}


void ucanopen_server_exec(const char* server_name, const char* category, const char* subcategory, const char* name) {
    ucanopen_client->server(server_name)->exec(category, subcategory, name);
}


unsigned int ucanopen_server_get_serial_number(const char* server_name) {
    return ucanopen_client->server(server_name)->get_serial_number();
}


void ucanopen_server_read_string(const char* server_name, 
                const char* category, const char* subcategory, const char* name, unsigned int timeout_ms,
                char* retbuf, int bufsize) {
    std::string ret = ucanopen_client->server(server_name)->read_string(category, subcategory, name, std::chrono::milliseconds(timeout_ms));
    retbuf[0] = '\0';
    strncat(retbuf, ret.data(), bufsize-1);
}


void ucanopen_server_read_numval(const char* server_name, 
                const char* category, const char* subcategory, const char* name, unsigned int timeout_ms,
                char* retbuf, int bufsize) {
    std::string ret = ucanopen_client->server(server_name)->read_numval(category, subcategory, name, std::chrono::milliseconds(timeout_ms));
    retbuf[0] = '\0';
    strncat(retbuf, ret.data(), bufsize-1);
}

}

