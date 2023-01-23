#include "cansocket/cansocket.h"


std::shared_ptr<can::Socket> can_socket;


namespace api {
void register_can_socket(std::shared_ptr<can::Socket> can_socket_)
{
	can_socket = can_socket_;
}
}


extern "C" {

void cansocket_connect(const char* interface, int bitrate)
{
	if (can_socket == nullptr)
	{
		return;
	}

	can_socket->connect(interface, bitrate);
}


void cansocket_disconnect()
{
	if (can_socket == nullptr)
	{
		return;
	}

	can_socket->disconnect();
}

}

