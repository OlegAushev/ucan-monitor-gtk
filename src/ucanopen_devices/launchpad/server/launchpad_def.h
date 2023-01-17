/**
 * @file launchpad_def.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <cstdint>
#include <string>
#include <vector>


namespace launchpad {


struct CobTpdo4
{
	uint64_t counter : 2;
	uint64_t errors : 31;
	uint64_t warnings : 31;
};


struct CobRpdo1
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobRpdo2
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobRpdo3
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobRpdo4
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobClientTpdo1
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobClientTpdo2
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobClientTpdo3
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


struct CobClientTpdo4
{
	uint32_t counter : 2;
	uint32_t _reserved : 30;
	float value;
};


inline const std::vector<std::string> syslogMessages = {
	"No message",
	"[syslog] Boot CPU1...",
	"[syslog] Boot CPU1 - success.",
	"[syslog] Boot CPU2...",
	"[syslog] Boot CPU2 - success.",
	"[syslog] CPU1 is ready.",
	"[syslog] CPU2 is ready.",
	"[syslog] Device is ready.",
	"[syslog] Device is busy.",
	"[syslog] Resetting device...",
	"[syslog] SDO request lost.",
};


inline const std::vector<std::string_view> errorList = {
	"can_bus_connection_lost",
};


inline const std::vector<std::string_view> warningList = {
	"can_bus_error",
	"can_bus_overrun",
	"can_bus_connection_lost",
};


} // namespace launchpad


