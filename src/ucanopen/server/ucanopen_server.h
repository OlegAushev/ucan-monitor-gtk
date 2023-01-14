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

#include "ucanopen_impl_server.h"
#include "services/ucanopen_server_heartbeat.h"
#include "services/ucanopen_server_tpdo.h"
#include "services/ucanopen_server_watch.h"


namespace ucanopen {


class Server : public impl::Server
{
	friend class Client;
public:
	ServerHeartbeatService heartbeatService;
	ServerTpdoService tpdoService;
	ServerWatchService watchService;
	const std::string_view configCategory;

	/* RPDO server <-- client */
private:
	bool _isRpdoEnabled = false;
	struct RpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<RpdoType, RpdoInfo> _rpdoList;
protected:
	void _registerRpdo(RpdoType type, std::chrono::milliseconds period)
	{
		canid_t id = calculateCobId(toCobType(type), _nodeId);
		_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now()}});
	}
public:
	/**
	 * @brief 
	 * 
	 */
	void enableRpdo()
	{
		std::cout << "[ucanopen] Enabling '" << _name << "' server RPDO messages... ";
		_isRpdoEnabled = true;
		std::cout << "done." << std::endl;
	}
	
	/**
	 * @brief 
	 * 
	 */
	void disableRpdo() 
	{
		std::cout << "[ucanopen] Disabling '" << _name << "' server RPDO messages... ";
		_isRpdoEnabled = false;
		std::cout << "done." << std::endl;
	}

	/* TSDO server --> client */	

	/* CONFIGURATION entries */
private:
	std::map<std::string_view, std::vector<std::string_view>> _configEntriesList;
public:
	/**
	 * @brief 
	 * 
	 * @return std::map<std::string_view, std::vector<std::string_view>> 
	 */
	std::map<std::string_view, std::vector<std::string_view>> configEntriesList() const
	{
		return _configEntriesList;
	}

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


