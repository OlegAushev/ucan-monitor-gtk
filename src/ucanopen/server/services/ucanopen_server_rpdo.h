/**
 * @file ucanopen_server_rpdo.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"

#include <functional>


namespace ucanopen {


class ServerRpdoService
{
private:
	impl::Server* const _server;
	bool _isEnabled = false;

	struct Message
	{
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload(void)> creator;
	};
	std::map<RpdoType, Message> _rpdoList;
public:
	ServerRpdoService(impl::Server* server);
	void registerRpdo(RpdoType type, std::chrono::milliseconds period, std::function<can_payload(void)> creator);
	void updateNodeId();

	void enable()
	{
		std::cout << "[ucanopen] Enabling '" << _server->name() << "' server RPDO messages... ";
		_isEnabled = true;
		std::cout << "done." << std::endl;
	}

	void disable() 
	{
		std::cout << "[ucanopen] Disabling '" << _server->name() << "' server RPDO messages... ";
		_isEnabled = false;
		std::cout << "done." << std::endl;
	}

	void send()
	{
		if (_isEnabled)
		{
			for (auto& [type, message] : _rpdoList)
			{
				if (message.period == std::chrono::milliseconds(0)) continue;
				
				auto now = std::chrono::steady_clock::now();
				if (now - message.timepoint < message.period) continue;

				can_payload data = message.creator();
				_server->_socket->send(create_frame(message.id, 8, data));
				message.timepoint = now;	
			}
		}
	}
};


}


