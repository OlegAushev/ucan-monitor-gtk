#pragma once


#include "../impl/ucanopen_impl_server.h"
#include "ucanopen_server_watch.h"


namespace ucanopen {

class ServerSdoService : public impl::FrameHandlingService
{
private:
	impl::Server* const _server;
	ServerWatchService* const _watch_service;
	canid_t _id;
public:
	ServerSdoService(impl::Server* server, ServerWatchService* watch_service);
	void update_node_id();

	virtual HandlingStatus handle_frame(const can_frame& frame);
};

} // namespace ucanopen

