#pragma once


#include "../impl/ucanopen_impl_server.h"
#include "ucanopen_server_watch.h"


namespace ucanopen {

class ServerSdoService : public impl::FrameHandlingService, public impl::SdoPublisher
{
private:
	impl::Server* const _server;
	canid_t _id;
public:
	ServerSdoService(impl::Server* server);
	void update_node_id();

	virtual int handle_frame(const can_frame& frame);
private:
	int _handle_read_expedited(const can_frame& frame);
	int _handle_write_expedited(const can_frame& frame);
	int _handle_abort(const can_frame& frame);
};

} // namespace ucanopen

