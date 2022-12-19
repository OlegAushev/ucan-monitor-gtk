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
	std::string m_name{"unnamed"};
	NodeId m_nodeId;
	std::shared_ptr<can::Socket> m_socket;

private:
	/* OBJECT DICTIONARY */
	const ObjectDictionaryType& m_dictionary;
	ObjectDictionaryAuxType m_dictionaryAux;

	/* HEARTBEAT server --> client */
private:
	struct HeartbeatInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		NmtState nmtState;
	};
	HeartbeatInfo m_heartbeatInfo;
public:
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isHeartbeatOk() const
	{
		return ((std::chrono::steady_clock::now() - m_heartbeatInfo.timepoint) <= m_heartbeatInfo.timeout)
				&& (m_heartbeatInfo.nmtState == NmtState::Operational);
	}

	/* TPDO server --> client */
private:
	struct TpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoList;
protected:
	virtual void handleTpdo1(can_payload data) = 0;
	virtual void handleTpdo2(can_payload data) = 0;
	virtual void handleTpdo3(can_payload data) = 0;
	virtual void handleTpdo4(can_payload data) = 0;
	void registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
	{
		canid_t id = calculateCobId(toCobType(type), m_nodeId);
		m_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now()}});
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
		if (!m_tpdoList.contains(tpdo)) return false;
		return (std::chrono::steady_clock::now() - m_tpdoList.at(tpdo).timepoint) <= m_tpdoList.at(tpdo).timeout;
	}

	/* RPDO server <-- client */
private:
	bool m_isRpdoEnabled{false};
	struct RpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<RpdoType, RpdoInfo> m_rpdoList;
protected:
	virtual can_payload createRpdo1() = 0;
	virtual can_payload createRpdo2() = 0;
	virtual can_payload createRpdo3() = 0;
	virtual can_payload createRpdo4() = 0;
	void registerRpdo(RpdoType type, std::chrono::milliseconds period)
	{
		canid_t id = calculateCobId(toCobType(type), m_nodeId);
		m_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now()}});
	}
public:
	/**
	 * @brief 
	 * 
	 */
	void enableRpdo()
	{
		std::cout << "[ucanopen] Enabling '" << m_name << "' server RPDO messages... ";
		m_isRpdoEnabled = true;
		std::cout << "done." << std::endl;
	}
	
	/**
	 * @brief 
	 * 
	 */
	void disableRpdo() 
	{
		std::cout << "[ucanopen] Disabling '" << m_name << "' server RPDO messages... ";
		m_isRpdoEnabled = false;
		std::cout << "done." << std::endl;
	}

	/* TSDO server --> client */
protected:
	virtual void handleTsdo(SdoType, ObjectDictionaryType::const_iterator, CobSdoData) = 0;

	/* WATCH messages server <- client */
private:
	bool m_isWatchEnabled{false};
	struct WatchInfo
	{
		std::chrono::milliseconds period{std::chrono::milliseconds(1000)};
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	WatchInfo m_watchInfo;
	std::vector<std::string_view> m_watchEntriesList;
	mutable std::mutex m_watchMutex;
protected:
	std::map<std::string_view, std::string> m_watchData;
public:
	/**
	 * @brief 
	 * 
	 */
	void enableWatch()
	{
		std::cout << "[ucanopen] Enabling '" << m_name << "' server watch requests... ";
		m_isWatchEnabled = true;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disableWatch()
	{
		std::cout << "[ucanopen] Disabling '" << m_name << "' server watch requests... ";
		m_isWatchEnabled = false;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief
	 * 
	 * @param period 
	 */
	void setWatchPeriod(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting '" << m_name << "' server watch requests period = " << period << "... ";
		m_watchInfo.period = period;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 * @return std::vector<std::string_view> 
	 */
	std::vector<std::string_view> watchEntriesList() const
	{
		return m_watchEntriesList;
	}

	/**
	 * @brief 
	 * 
	 * @param watchName 
	 * @return std::string 
	 */
	std::string watchValue(std::string_view watchName) const
	{
		auto it = m_watchData.find(watchName);
		if (it == m_watchData.end())
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
		auto it = m_watchData.find(watchName);
		if (it == m_watchData.end())
		{
			const char* str = "n/a";
			std::strncpy(buf, str, len);
			return;
		}
		std::lock_guard<std::mutex> lock(m_watchMutex);
		std::strncpy(buf, it->second.c_str(), len);
	}

	/* CONFIGURATION entries */
private:
	std::map<std::string_view, std::vector<std::string_view>> m_confEntriesList;
public:
	/**
	 * @brief 
	 * 
	 * @return std::map<std::string_view, std::vector<std::string_view>> 
	 */
	std::map<std::string_view, std::vector<std::string_view>> confEntriesList() const
	{
		return m_confEntriesList;
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
		return m_name;
	}

	/**
	 * @brief Returns server node ID.
	 * 
	 * @return NodeId 
	 */
	NodeId nodeId() const
	{
		return m_nodeId;
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
	NmtState nmtState() const { return m_heartbeatInfo.nmtState; }

private:	
	std::map<ODEntryKey, ODEntryValue>::const_iterator findOdEntry(
		std::string_view category,
		std::string_view subcategory,
		std::string_view name)
	{
		auto it = m_dictionaryAux.find({category, subcategory, name});
		if (it == m_dictionaryAux.end())
		{
			return m_dictionary.end();
		}
		return it->second;
	}

	void sendPeriodic();
	void handleFrame(const can_frame& frame);

	/**
	 * @brief Sets server node ID.
	 * 
	 * @param nodeId 
	 */
	void setNodeId(NodeId nodeId);
};


} // namespace ucanopen


