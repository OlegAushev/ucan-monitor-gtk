#pragma once


#include "../impl/ucanopen_impl_server.h"


namespace ucanopen {

class ServerHeartbeatService : public impl::FrameHandlingService
{
private:
	impl::Server* const _server;
	canid_t _id;
	std::chrono::milliseconds _timeout;
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
public:
	ServerHeartbeatService(impl::Server* server, std::chrono::milliseconds timeout);

	bool is_ok() const
	{
		return ((std::chrono::steady_clock::now() - _timepoint) <= _timeout)
				&& (_server->nmt_state() == NmtState::operational);
	}

	void update_node_id();

	virtual HandlingStatus handle_frame(const can_frame& frame)
	{
		if (frame.can_id != _id) return HandlingStatus::invalid_id;
		
		_timepoint = std::chrono::steady_clock::now();
		_server->_nmt_state = static_cast<NmtState>(frame.data[0]);
		return HandlingStatus::success;
	}
};

} // namespace ucanopen

