#include "log.h"


extern "C" {

bool log_get_message(char* buf, size_t len)
{
	std::string message = Log().pop();
	if (message.empty())
	{
		return false;
	}
	strncpy(buf, message.c_str(), len);
	return true;
} 

}

