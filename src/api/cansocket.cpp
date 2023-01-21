#include "cansocket/cansocket.h"


namespace global {
extern std::shared_ptr<can::Socket> can_socket;
}


extern "C" {

void cansocket_connect(const char* interface, int bitrate)
{
	if (global::can_socket == nullptr)
	{
		return;
	}

	global::can_socket->connect(interface, bitrate);
}


void cansocket_disconnect()
{
	if (global::can_socket == nullptr)
	{
		return;
	}

	global::can_socket->disconnect();
}

}

