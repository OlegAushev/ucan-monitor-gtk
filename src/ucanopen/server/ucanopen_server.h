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
	static constexpr std::string_view confSubcategory = "conf";

private:
	std::string m_name{"unnamed"};
	NodeId m_nodeId;
	std::shared_ptr<can::Socket> m_socket;
	bool m_isConnectionOk{false};

private:
	/* OBJECT DICTIONARY */
	const ObjectDictionaryType& m_dictionary;
	ObjectDictionaryAuxType m_dictionaryAux;

	/* TPDO server --> client */
private:
	struct TpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool isOnSchedule;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoList;
protected:
	virtual void handleTpdo1(can_payload data) = 0;
	virtual void handleTpdo2(can_payload data) = 0;
	virtual void handleTpdo3(can_payload data) = 0;
	virtual void handleTpdo4(can_payload data) = 0;
	void registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
	{
		canid_t id = calculateCobId(toCobType(type), m_nodeId.value());
		m_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now(), false}});
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
		canid_t id = calculateCobId(toCobType(type), m_nodeId.value());
		m_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now()}});
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
public:
	/**
	 * @brief 
	 * 
	 */
	void enableWatch()
	{
		std::cout << "[ucanopen] Enabling " << m_name << " server watch requests... ";
		m_isWatchEnabled = true;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disableWatch()
	{
		std::cout << "[ucanopen] Disabling " << m_name << " server watch requests... ";
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
		std::cout << "[ucanopen] Setting " << m_name << " server watch requests period = " << period << "... ";
		m_watchInfo.period = period;
		std::cout << "done." << std::endl;
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
	 */
	void enableRpdo()
	{
		std::cout << "[ucanopen] Enabling " << m_name << " server RPDO messages... ";
		m_isRpdoEnabled = true;
		std::cout << "done." << std::endl;
	}
	
	/**
	 * @brief 
	 * 
	 */
	void disableRpdo() 
	{
		std::cout << "[ucanopen] Disabling " << m_name << " server RPDO messages... ";
		m_isRpdoEnabled = false;
		std::cout << "done." << std::endl;
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
	 * @return std::vector<std::string_view> 
	 */
	std::vector<std::string_view> watchEntriesList() const
	{
		return m_watchEntriesList;
	}

	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isConnectionOk() const { return m_isConnectionOk; }

	/**
	 * @brief 
	 * 
	 * @param tpdo 
	 * @return true 
	 * @return false 
	 */
	bool isTpdoOk(TpdoType tpdo) const { return m_tpdoList.at(tpdo).isOnSchedule; }

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
	void checkConnection();
};


} // namespace ucanopen


