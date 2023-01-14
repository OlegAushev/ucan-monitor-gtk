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
#include "services/ucanopen_server_watch.h"


namespace ucanopen {


class Server : public impl::Server
{
	friend class Client;
public:
	ServerHeartbeatService heartbeatService;
	ServerWatchService watchService;
	const std::string_view configCategory;

	/* TPDO server --> client */
private:
	struct TpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		can_payload data;
	};
	std::map<TpdoType, TpdoInfo> _tpdoList;
protected:
	void _registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
	{
		canid_t id = calculateCobId(toCobType(type), _nodeId);
		_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now(), can_payload{}}});
	}
public:
	/**
	 * @brief 
	 * 
	 * @param tpdo 
	 * @return true 
	 * @return false 
	 */
	bool isTpdoOk(TpdoType tpdo) const
	{
		if (!_tpdoList.contains(tpdo)) return false;
		return (std::chrono::steady_clock::now() - _tpdoList.at(tpdo).timepoint) <= _tpdoList.at(tpdo).timeout;
	}

	/**
	 * @brief 
	 * 
	 * @param tpdo 
	 * @return can_payload 
	 */
	can_payload tpdoData(TpdoType tpdo) const
	{
		if (!_tpdoList.contains(tpdo)) return can_payload{};
		return _tpdoList.at(tpdo).data;
	}

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
	/**
	 * @brief Construct a new IServer object
	 * 
	 * @param nodeId_ 
	 * @param socket 
	 * @param dictionary 
	 */
	Server(const std::string& name, NodeId nodeId_, std::shared_ptr<can::Socket> socket,
			const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig);
	
	/**
	 * @brief Destroy the Server object
	 * 
	 */
	virtual ~Server() = default;
	
private:	
	void _sendPeriodic();
	void _handleFrame(const can_frame& frame);

	/**
	 * @brief Sets server node ID.
	 * 
	 * @param nodeId 
	 */
	void _setNodeId(NodeId nodeId);
};


} // namespace ucanopen


