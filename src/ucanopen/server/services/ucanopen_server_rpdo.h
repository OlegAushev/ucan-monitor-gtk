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
#include "../ucanopen_impl_server.h"


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
	};
	std::map<RpdoType, Message> _rpdoList;
public:
	ServerRpdoService(impl::Server* server);
	void registerRpdo(RpdoType type, std::chrono::milliseconds period);
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

	void sendPeriodic()
	{
		if (_isEnabled)
		{
			for (auto& [type, message] : _rpdoList)
			{
				if (message.period == std::chrono::milliseconds(0)) continue;
				
				auto now = std::chrono::steady_clock::now();
				if (now - message.timepoint < message.period) continue;

				can_payload data;
				switch (type)
				{
				case RpdoType::Rpdo1:
					data = _server->_createRpdo1();
					break;
				case RpdoType::Rpdo2:
					data = _server->_createRpdo2();
					break;
				case RpdoType::Rpdo3:
					data = _server->_createRpdo3();
					break;
				case RpdoType::Rpdo4:
					data = _server->_createRpdo4();
					break;
				}
				_server->_socket->send(createFrame(message.id, 8, data));
				message.timepoint = now;	
			}
		}
	}
};


}


