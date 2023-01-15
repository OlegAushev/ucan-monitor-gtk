/**
 * @file ucanopen_service_heartbeat.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"


namespace ucanopen {


class ServerHeartbeatService
{
private:
	impl::Server* const _server;
	canid_t _id;
	std::chrono::milliseconds _timeout;
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
public:
	ServerHeartbeatService(impl::Server* server, std::chrono::milliseconds timeout);

	bool isOk() const
	{
		return ((std::chrono::steady_clock::now() - _timepoint) <= _timeout)
				&& (_server->nmtState() == NmtState::Operational);
	}

	void updateNodeId();

	bool handleFrame(const can_frame& frame)
	{
		if (frame.can_id != _id) return false;
		
		_timepoint = std::chrono::steady_clock::now();
		_server->_nmtState = static_cast<NmtState>(frame.data[0]);
		return true;
	}
};


}


