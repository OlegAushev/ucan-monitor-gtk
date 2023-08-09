#include "atvvcu_server.h"


namespace atvvcu {

using namespace ucanopen;


extern const ucanopen::ObjectDictionary object_dictionary = {
.config = {
    .watch_category = "watch",
    .config_category = "config"
},
.entries = {
{{0x1008, 0x00}, {"info", "sys", "device_name", "", OD_STRING, OD_ACCESS_CONST}},
{{0x1009, 0x00}, {"info", "sys", "hardware_version", "", OD_STRING, OD_ACCESS_CONST}},
{{0x100A, 0x00}, {"info", "sys", "firmware_version", "", OD_STRING, OD_ACCESS_CONST}},

{{0x1010, 0x01}, {"ctl", "sys", "save_all_parameters", "", OD_EXEC, OD_ACCESS_WO}},
{{0x1011, 0x01}, {"ctl", "sys", "restore_all_default_parameters", "", OD_EXEC, OD_ACCESS_WO}},

{{0x1018, 0x04}, {"info", "sys", "serial_number", "", OD_UINT32, OD_ACCESS_CONST}},

{{0x2000, 0x01}, {"ctl", "sys", "reset_device", "", OD_EXEC, OD_ACCESS_WO}},
{{0x2000, 0x02}, {"ctl", "sys", "clear_errors", "", OD_EXEC, OD_ACCESS_WO}},

{{0x2001, 0x01}, {"ctl", "pdm", "contactor_enable", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2001, 0x02}, {"ctl", "pdm", "contactor_disable", "", OD_UINT32, OD_ACCESS_WO}},

{{0x2002, 0x01}, {"ctl", "motordrives", "control_mode", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x02}, {"ctl", "motordrives", "enable_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x03}, {"ctl", "motordrives", "active_discharge_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x04}, {"ctl", "motordrives", "mainrelay_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x05}, {"ctl", "motordrives", "gear", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x06}, {"ctl", "motordrives", "footbrake_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x07}, {"ctl", "motordrives", "handbrake_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x08}, {"ctl", "motordrives", "faultreset_status", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x09}, {"ctl", "motordrives", "speed_ref", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x0A}, {"ctl", "motordrives", "torque_ref", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x0B}, {"ctl", "motordrives", "eltorque_max", "", OD_UINT32, OD_ACCESS_WO}},
{{0x2002, 0x0C}, {"ctl", "motordrives", "braketorque_max", "", OD_UINT32, OD_ACCESS_WO}},


{{0x5000, 0x01}, {"watch", "sys", "uptime", "s", OD_FLOAT32, OD_ACCESS_RO}},
{{0x5000, 0x02}, {"watch", "sys", "syslog_message", "", OD_UINT32, OD_ACCESS_RO}},
{{0x5000, 0x03}, {"watch", "pdm", "pdm_contactor_state", "", OD_UINT32, OD_ACCESS_RO}},


}
};

} // namespace atvvcu

