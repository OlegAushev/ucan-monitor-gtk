/**
 * @file ucanopen_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <atomic>
#include <functional>
#include <chrono>
#include <future>

#include "impl/ucanopen_impl_server.h"
#include "services/ucanopen_server_heartbeat.h"
#include "services/ucanopen_server_tpdo.h"
#include "services/ucanopen_server_rpdo.h"
#include "services/ucanopen_server_watch.h"
#include "services/ucanopen_server_config.h"


namespace ucanopen {


class Server : public impl::Server
{
	friend class Client;
public:
	ServerHeartbeatService heartbeatService;
	ServerTpdoService tpdoService;
	ServerRpdoService rpdoService;
	ServerWatchService watchService;
	ServerConfigService configService;

public:
	Server(const std::string& name, NodeId nodeId_, std::shared_ptr<can::Socket> socket,
			const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig);
	virtual ~Server() = default;
private:	
	void _sendPeriodic();
	void _handleFrame(const can_frame& frame);
	void _setNodeId(NodeId nodeId);
};


} // namespace ucanopen


