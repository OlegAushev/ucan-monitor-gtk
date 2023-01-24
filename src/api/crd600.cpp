#include <iostream>
#include <cassert>
#include "ucanopen_devices/crd600/server/crd600_server.h"


std::shared_ptr<crd600::Server> crd600_server;


namespace api {
void register_crd600_server(std::shared_ptr<crd600::Server> crd600_server_)
{
	crd600_server = crd600_server_;
}
}


extern "C" {



}

