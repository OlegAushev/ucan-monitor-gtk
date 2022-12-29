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

#include "../ucanopen_def.h" 
#include "cansocket/cansocket.h"


namespace ucanopen {


class IServer
{
	friend class Client;
public:
	static constexpr std::string_view watchCategory = "WATCH";
	static constexpr std::string_view watchSubcategory = "WATCH";
	static constexpr std::string_view confCategory = "CONFIG";

private:
	std::string _name = "unnamed";
	NodeId _nodeId;
	std::shared_ptr<can::Socket> _socket;

private:
	/* OBJECT DICTIONARY */
	const ObjectDictionaryType& _dictionary;
	ObjectDictionaryAuxType _dictionaryAux;

	/* HEARTBEAT server --> client */
private:
	struct HeartbeatInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		NmtState nmtState;
	};
	HeartbeatInfo _heartbeatInfo;
public:
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isHeartbeatOk() const
	{
		return ((std::chrono::steady_clock::now() - _heartbeatInfo.timepoint) <= _heartbeatInfo.timeout)
				&& (_heartbeatInfo.nmtState == NmtState::Operational);
	}

	/* TPDO server --> client */
private:
	struct TpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<TpdoType, TpdoInfo> _tpdoList;
protected:
	virtual void _handleTpdo1(can_payload data) = 0;
	virtual void _handleTpdo2(can_payload data) = 0;
	virtual void _handleTpdo3(can_payload data) = 0;
	virtual void _handleTpdo4(can_payload data) = 0;
	void _registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
	{
		canid_t id = calculateCobId(toCobType(type), _nodeId);
		_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now()}});
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
	virtual can_payload _createRpdo1() = 0;
	virtual can_payload _createRpdo2() = 0;
	virtual can_payload _createRpdo3() = 0;
	virtual can_payload _createRpdo4() = 0;
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
protected:
	virtual void _handleTsdo(SdoType, ObjectDictionaryType::const_iterator, CobSdoData) = 0;

	/* WATCH messages server <- client */
private:
	bool _isWatchEnabled = false;
	struct WatchInfo
	{
		std::chrono::milliseconds period = std::chrono::milliseconds(1000);
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	WatchInfo _watchInfo;
	std::vector<std::string_view> _watchEntriesList;
	mutable std::mutex _watchMutex;
protected:
	std::map<std::string_view, std::string> _watchData;
public:
	/**
	 * @brief 
	 * 
	 */
	void enableWatch()
	{
		std::cout << "[ucanopen] Enabling '" << _name << "' server watch requests (period = " << _watchInfo.period << ")... ";
		_isWatchEnabled = true;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disableWatch()
	{
		std::cout << "[ucanopen] Disabling '" << _name << "' server watch requests... ";
		_isWatchEnabled = false;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief
	 * 
	 * @param period 
	 */
	void setWatchPeriod(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting '" << _name << "' server watch requests period = " << period << "... ";
		_watchInfo.period = period;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 * @return std::vector<std::string_view> 
	 */
	std::vector<std::string_view> watchEntriesList() const
	{
		return _watchEntriesList;
	}

	/**
	 * @brief 
	 * 
	 * @param watchName 
	 * @return std::string 
	 */
	std::string watchValue(std::string_view watchName) const
	{
		auto it = _watchData.find(watchName);
		if (it == _watchData.end())
		{
			return "n/a";
		}
		return it->second;
	}

	/**
	 * @brief 
	 * 
	 * @param watchName 
	 * @param buf 
	 * @param len 
	 */
	void watchValue(std::string_view watchName, char* buf, size_t len) const
	{
		auto it = _watchData.find(watchName);
		if (it == _watchData.end())
		{
			const char* str = "n/a";
			std::strncpy(buf, str, len);
			return;
		}
		std::lock_guard<std::mutex> lock(_watchMutex);
		std::strncpy(buf, it->second.c_str(), len);
	}

	/* CONFIGURATION entries */
private:
	std::map<std::string_view, std::vector<std::string_view>> _confEntriesList;
public:
	/**
	 * @brief 
	 * 
	 * @return std::map<std::string_view, std::vector<std::string_view>> 
	 */
	std::map<std::string_view, std::vector<std::string_view>> confEntriesList() const
	{
		return _confEntriesList;
	}

public:
	/**
	 * @brief Construct a new IServer object
	 * 
	 * @param nodeId_ 
	 * @param socket 
	 * @param dictionary 
	 */
	IServer(const std::string& name, NodeId nodeId_, std::shared_ptr<can::Socket> socket, const ObjectDictionaryType& dictionary);
	
	/**
	 * @brief Destroy the IServer object
	 * 
	 */
	virtual ~IServer() = default;

	/**
	 * @brief Returns server name.
	 * 
	 * @return std::string_view 
	 */
	std::string_view name() const
	{
		return _name;
	}

	/**
	 * @brief Returns server node ID.
	 * 
	 * @return NodeId 
	 */
	NodeId nodeId() const
	{
		return _nodeId;
	}
	
	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @param data 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @param value 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);

	/**
	 * @brief 
	 * 
	 * @return NmtState 
	 */
	NmtState nmtState() const { return _heartbeatInfo.nmtState; }

private:	
	std::map<ODEntryKey, ODEntryValue>::const_iterator _findOdEntry(
		std::string_view category,
		std::string_view subcategory,
		std::string_view name)
	{
		auto it = _dictionaryAux.find({category, subcategory, name});
		if (it == _dictionaryAux.end())
		{
			return _dictionary.end();
		}
		return it->second;
	}

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


