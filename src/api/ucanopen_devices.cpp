/**
 * @file ucanopen_devices.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"


namespace global {
extern std::shared_ptr<bmsmain::Server> bmsmainServer;
extern std::shared_ptr<srmdrive::Server> srmdriveServer;
}


extern "C" {


///
///
///
size_t ucanopen_devices_get_error_names(const char* serverName, char** buf, size_t countMax, size_t lenMax)
{
	if (std::string(serverName) == "SRM Drive")
	{
		if (srmdrive::errorList.size() > countMax)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : srmdrive::errorList)
		{
			strncpy(buf[i++], error.data(), lenMax);
		}

		return srmdrive::errorList.size();
	}
	else if (std::string(serverName) == "LaunchPad")
	{
		// TODO
	}
	else if (std::string(serverName) == "BMS Main")
	{
		// TODO
	}

	return 0;
}


///
///
///
size_t ucanopen_devices_get_warning_names(const char* serverName, char** buf, size_t countMax, size_t lenMax)
{
	if (std::string(serverName) == "SRM Drive")
	{
		if (srmdrive::warningList.size() > countMax)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : srmdrive::warningList)
		{
			strncpy(buf[i++], error.data(), lenMax);
		}

		return srmdrive::warningList.size();
	}
	else if (std::string(serverName) == "LaunchPad")
	{
		// TODO
	}
	else if (std::string(serverName) == "BMS Main")
	{
		// TODO
	}

	return 0;
}


///
///
///
unsigned int ucanopen_devices_get_error_code(const char* serverName)
{
	if (std::string(serverName) == "SRM Drive")
	{
		return global::srmdriveServer->errors();
	}
	else if (std::string(serverName) == "LaunchPad")
	{
		// TODO
	}
	else if (std::string(serverName) == "BMS Main")
	{
		// TODO
	}

	return 0;	
}


///
///
///
unsigned int ucanopen_devices_get_warning_code(const char* serverName)
{
	if (std::string(serverName) == "SRM Drive")
	{
		return global::srmdriveServer->warnings();
	}
	else if (std::string(serverName) == "LaunchPad")
	{
		// TODO
	}
	else if (std::string(serverName) == "BMS Main")
	{
		// TODO
	}

	return 0;	
}


}


