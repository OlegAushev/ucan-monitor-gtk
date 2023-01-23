#pragma once


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
	std::map<TpdoType, Message> _tpdo_list;
public:
	ServerTpdoService(impl::Server* server);
	void register_tpdo(TpdoType tpdo_type, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler);
	void update_node_id();

	bool is_ok(TpdoType tpdo_type) const
	{
		if (!_tpdo_list.contains(tpdo_type)) return false;
		return (std::chrono::steady_clock::now() - _tpdo_list.at(tpdo_type).timepoint) <= _tpdo_list.at(tpdo_type).timeout;
	}

	can_payload data(TpdoType tpdo_type) const
	{
		if (!_tpdo_list.contains(tpdo_type)) return can_payload{};
		return _tpdo_list.at(tpdo_type).data;
	}

	bool handle_frame(const can_frame& frame)
	{
		for (auto& [tpdo_type, message] : _tpdo_list)
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

