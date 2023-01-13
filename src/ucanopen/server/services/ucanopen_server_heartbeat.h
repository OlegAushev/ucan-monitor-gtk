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


#include "../../ucanopen_def.h" 
#include "cansocket/cansocket.h"


namespace ucanopen {


class ServerHeartbeatService
{
private:
	canid_t _id;
	std::chrono::milliseconds _timeout;
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
	NmtState _nmtState;
public:
	ServerHeartbeatService(NodeId nodeId, std::chrono::milliseconds timeout);

	bool isOk() const
	{
		return ((std::chrono::steady_clock::now() - _timepoint) <= _timeout)
				&& (_nmtState == NmtState::Operational);
	}

	NmtState nmtState() const { return _nmtState; }

	void setNodeId(NodeId nodeId);

	bool handleFrame(const can_frame& frame)
	{
		if (frame.can_id != _id) return false;
		
		_timepoint = std::chrono::steady_clock::now();
		_nmtState = static_cast<NmtState>(frame.data[0]);
		return true;
	}
};


}


