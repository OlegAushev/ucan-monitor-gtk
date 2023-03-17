#include "launchpad_server.h"


namespace launchpad {

using namespace ucanopen;


extern const ucanopen::ObjectDictionary object_dictionary = {
.config = {
    .watch_category = "watch",
    .watch_subcategory = "watch",
    .config_category = "config"
},
.entries = {
{{0x1008, 0x00}, {"sys", "info", "device_name", "", OD_STRING, OD_ACCESS_CONST}},
{{0x1009, 0x00}, {"sys", "info", "hardware_version", "", OD_STRING, OD_ACCESS_CONST}},
{{0x100A, 0x00}, {"sys", "info", "firmware_version", "", OD_STRING, OD_ACCESS_CONST}},

{{0x1010, 0x01}, {"sys", "ctl", "save_all_parameters", "", OD_EXEC, OD_ACCESS_WO}},
{{0x1011, 0x01}, {"sys", "ctl", "restore_all_default_parameters", "", OD_EXEC, OD_ACCESS_WO}},

{{0x1018, 0x04}, {"sys", "info", "serial_number", "", OD_UINT32, OD_ACCESS_CONST}},

{{0x2000, 0x01}, {"sys", "ctl", "reset_device", "", OD_EXEC, OD_ACCESS_WO}},
{{0x2000, 0x02}, {"sys", "ctl", "reset_errors", "", OD_EXEC, OD_ACCESS_WO}},

{{0x5000, 0x01}, {"watch", "watch", "uptime", "s", OD_FLOAT32, OD_ACCESS_RO}},
{{0x5000, 0x02}, {"watch", "watch", "syslog_message", "", OD_UINT32, OD_ACCESS_RO}},

{{0x3000, 0x01}, {"config", "group 1", "parameter 1", "", OD_UINT32, OD_ACCESS_RW}},
{{0x3000, 0x02}, {"config", "group 1", "parameter 2", "", OD_FLOAT32, OD_ACCESS_RW}},
{{0x3001, 0x01}, {"config", "group 2", "parameter 3", "", OD_UINT32, OD_ACCESS_RW}},
{{0x3001, 0x02}, {"config", "group 2", "parameter 4", "", OD_FLOAT32, OD_ACCESS_RW}},
{{0x3001, 0x03}, {"config", "group 2", "parameter 5", "", OD_UINT32, OD_ACCESS_RW}},
}
};

} // namespace launchpad

