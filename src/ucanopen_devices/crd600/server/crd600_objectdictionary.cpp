#include "crd600_server.h"


namespace crd600 {

using namespace ucanopen;


extern const ucanopen::ObjectDictionary object_dictionary = {
.config = {
    .watch_category = "watch",
    .watch_subcategory = "watch",
    .config_category = "config"
},
.entries = {
{{0x1008, 0x00}, {"system", "info", "device_name", "", OD_STRING, OD_ACCESS_RO}},
{{0x5FFF, 0x00}, {"system", "info", "firmware_version", "", OD_STRING, OD_ACCESS_RO}},
{{0x5FFF, 0x01}, {"system", "info", "build_configuration", "", OD_STRING, OD_ACCESS_RO}},

{{0x2000, 0x00}, {"system", "device", "reset_device", "", OD_EXEC, OD_ACCESS_RO}},
{{0x2000, 0x01}, {"system", "syslog", "reset_errors", "", OD_EXEC, OD_ACCESS_RO}},
{{0x2000, 0x02}, {"system", "config", "reset_to_default", "", OD_EXEC, OD_ACCESS_RO}},
{{0x2000, 0x03}, {"system", "config", "apply", "", OD_EXEC, OD_ACCESS_RO}},

{{0x3000, 0x00}, {"control", "drive", "powerup", "", OD_EXEC, OD_ACCESS_RO}},
{{0x3000, 0x01}, {"control", "drive", "powerdown", "", OD_EXEC, OD_ACCESS_RO}},

{{0x5000, 0x00}, {"watch", "watch", "uptime", "s", OD_FLOAT32, OD_ACCESS_RO}},
{{0x5000, 0x01}, {"watch", "watch", "syslog_message", "", OD_UINT32, OD_ACCESS_RO}},
}
};

} // namespace crd600

