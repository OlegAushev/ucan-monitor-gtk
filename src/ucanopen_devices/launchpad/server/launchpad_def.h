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


namespace launchpad {


struct CobRpdo1
{
	double value;
};


struct CobRpdo2
{
	double value;
};


struct CobRpdo3
{
	double value;
};


struct CobRpdo4
{
	double value;
};


struct CobClientTpdo1
{
	double value;
};


struct CobClientTpdo2
{
	double value;
};


struct CobClientTpdo3
{
	double value;
};


struct CobClientTpdo4
{
	double value;
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


} // namespace launchpad


