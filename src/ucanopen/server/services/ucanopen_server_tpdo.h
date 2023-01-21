#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"

#include <functional>


namespace ucanopen {

class ServerTpdoService
{
private:
	impl::Server* const _server;

	struct Message
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		can_payload data;
		std::function<void(can_payload)> handler;
	};
	std::map<TpdoType, Message> _tpdoList;
public:
	ServerTpdoService(impl::Server* server);
	void registerTpdo(TpdoType type, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler);
	void update_node_id();

	bool isOk(TpdoType tpdo) const
	{
		if (!_tpdoList.contains(tpdo)) return false;
		return (std::chrono::steady_clock::now() - _tpdoList.at(tpdo).timepoint) <= _tpdoList.at(tpdo).timeout;
	}

	can_payload data(TpdoType tpdo) const
	{
		if (!_tpdoList.contains(tpdo)) return can_payload{};
		return _tpdoList.at(tpdo).data;
	}

	bool handle_frame(const can_frame& frame)
	{
		for (auto& [type, message] : _tpdoList)
		{
			if (frame.can_id != message.id) continue;

			message.timepoint = std::chrono::steady_clock::now();
			can_payload data{};
			std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
			message.data = data;
			message.handler(data);
			return true;
		}
		return false;
	}
};

} // namespace ucanopen

