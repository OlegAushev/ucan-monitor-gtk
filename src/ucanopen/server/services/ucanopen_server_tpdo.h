/**
 * @file ucanopen_server_tpdo.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"


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
	};
	std::map<TpdoType, Message> _tpdoList;
public:
	ServerTpdoService(impl::Server* server);
	void registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0));
	void updateNodeId();

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

	bool handleFrame(const can_frame& frame)
	{
		for (auto& [type, message] : _tpdoList)
		{
			if (frame.can_id != message.id) continue;

			message.timepoint = std::chrono::steady_clock::now();
			can_payload data{};
			std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
			message.data = data;

			switch (type)
			{
			case TpdoType::Tpdo1:
				_server->_handleTpdo1(data);
				return true;
			case TpdoType::Tpdo2:
				_server->_handleTpdo2(data);
				return true;
			case TpdoType::Tpdo3:
				_server->_handleTpdo3(data);
				return true;
			case TpdoType::Tpdo4:
				_server->_handleTpdo4(data);
				return true;
			}
			return false;
		}
	}
};


}


