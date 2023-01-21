#include "logger.h"


extern "C" {

bool logger_get_message(char* buf, size_t len)
{
	std::string message = Logger::instance().pop();
	if (message.empty())
	{
		return false;
	}
	strncpy(buf, message.c_str(), len);
	return true;
} 

}

