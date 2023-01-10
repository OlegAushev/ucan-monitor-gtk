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


