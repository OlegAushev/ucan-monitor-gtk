#include "ucanopen_devices/bmsmain/server/bmsmain_server.h"
#include "ucanopen_devices/launchpad/server/launchpad_server.h"
#include "ucanopen_devices/srmdrive/server/srmdrive_server.h"


namespace global {
extern std::shared_ptr<bmsmain::Server> bmsmain_server;
extern std::shared_ptr<launchpad::Server> launchpad_server;
extern std::shared_ptr<srmdrive::Server> srmdrive_server;
}


extern "C" {

size_t ucanopen_devices_get_error_names(const char* server_name, char** buf, size_t count_max, size_t len_max)
{
	if (std::string(server_name) == "SRM Drive")
	{
		if (srmdrive::error_list.size() > count_max)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : srmdrive::error_list)
		{
			strncpy(buf[i++], error.data(), len_max);
		}

		return srmdrive::error_list.size();
	}
	else if (std::string(server_name) == "LaunchPad")
	{
		if (launchpad::error_list.size() > count_max)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : launchpad::error_list)
		{
			strncpy(buf[i++], error.data(), len_max);
		}

		return launchpad::error_list.size();
	}
	else if (std::string(server_name) == "BMS Main")
	{
		// TODO
	}

	return 0;
}

size_t ucanopen_devices_get_warning_names(const char* server_name, char** buf, size_t count_max, size_t len_max)
{
	if (std::string(server_name) == "SRM Drive")
	{
		if (srmdrive::warning_list.size() > count_max)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : srmdrive::warning_list)
		{
			strncpy(buf[i++], error.data(), len_max);
		}

		return srmdrive::warning_list.size();
	}
	else if (std::string(server_name) == "LaunchPad")
	{
		if (launchpad::warning_list.size() > count_max)
		{
			return 0;
		}

		size_t i = 0;
		for (auto error : launchpad::warning_list)
		{
			strncpy(buf[i++], error.data(), len_max);
		}

		return launchpad::warning_list.size();
	}
	else if (std::string(server_name) == "BMS Main")
	{
		// TODO
	}

	return 0;
}

unsigned int ucanopen_devices_get_error_code(const char* server_name)
{
	if (std::string(server_name) == "SRM Drive")
	{
		return global::srmdrive_server->errors();
	}
	else if (std::string(server_name) == "LaunchPad")
	{
		return global::launchpad_server->errors();
	}
	else if (std::string(server_name) == "BMS Main")
	{
		// TODO
	}

	return 0;	
}

unsigned int ucanopen_devices_get_warning_code(const char* server_name)
{
	if (std::string(server_name) == "SRM Drive")
	{
		return global::srmdrive_server->warnings();
	}
	else if (std::string(server_name) == "LaunchPad")
	{
		return global::launchpad_server->warnings();
	}
	else if (std::string(server_name) == "BMS Main")
	{
		// TODO
	}

	return 0;	
}

}

