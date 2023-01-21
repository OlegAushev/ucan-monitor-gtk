/**
 * @file launchpad_objectdictionary.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "launchpad_server.h"


namespace launchpad {


using namespace ucanopen;


extern const ucanopen::ObjectDictionaryConfig objectDictionaryConfig = {
	.watch_category = "watch",
	.watch_subcategory = "watch",
	.config_category = "config"
};


extern const ucanopen::ObjectDictionary objectDictionary = {
{{0x1008, 0x00}, {"system", "info", "device_name", "", OD_STRING_4CHARS, OD_ACCESS_RO}},
{{0x5FFF, 0x00}, {"system", "info", "firmware_version", "", OD_STRING_4CHARS, OD_ACCESS_RO}},
{{0x5FFF, 0x01}, {"system", "info", "build_configuration", "", OD_STRING_4CHARS, OD_ACCESS_RO}},

{{0x2000, 0x00}, {"system", "device", "reset_device", "", OD_EXEC, OD_ACCESS_RO}},
{{0x2000, 0x01}, {"system", "syslog", "reset_errors", "", OD_EXEC, OD_ACCESS_RO}},

{{0x5000, 0x00}, {"watch", "watch", "uptime", "s", OD_FLOAT32, OD_ACCESS_RO}},
{{0x5000, 0x01}, {"watch", "watch", "syslog_message", "", OD_UINT32, OD_ACCESS_RO}},
};


} // namespace launchpad


