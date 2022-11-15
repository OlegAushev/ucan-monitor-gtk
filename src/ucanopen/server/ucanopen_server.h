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
	NodeId nodeId;
private:
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
	virtual void handleTpdo1(std::array<uint8_t, 8> data) = 0;
	virtual void handleTpdo2(std::array<uint8_t, 8> data) = 0;
	virtual void handleTpdo3(std::array<uint8_t, 8> data) = 0;
	virtual void handleTpdo4(std::array<uint8_t, 8> data) = 0;
	void registerTpdo(TpdoType type, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
	{
		canid_t id = calculateCobId(toCobType(type), nodeId.value());
		m_tpdoList.insert({type, {id, timeout, std::chrono::steady_clock::now(), false}});
	}

	/* RPDO server <-- client */
private:
	bool m_isRpdoEnabled;
	struct RpdoInfo
	{
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<RpdoType, RpdoInfo> m_rpdoList;
protected:
	virtual std::array<uint8_t, 8> createRpdo1() = 0;
	virtual std::array<uint8_t, 8> createRpdo2() = 0;
	virtual std::array<uint8_t, 8> createRpdo3() = 0;
	virtual std::array<uint8_t, 8> createRpdo4() = 0;
	void registerRpdo(RpdoType type, std::chrono::milliseconds period)
	{
		canid_t id = calculateCobId(toCobType(type), nodeId.value());
		m_rpdoList.insert({type, {id, period, std::chrono::steady_clock::now()}});
	}

	/* TSDO server --> client */
protected:
	virtual void handleTsdo(SdoType, ObjectDictionaryType::const_iterator, CobSdoData) = 0;

public:
	IServer(NodeId nodeId_, std::shared_ptr<can::Socket> socket, const ObjectDictionaryType& dictionary);
	void enableRpdo() { m_isRpdoEnabled = true; }
	void disableRpdo() { m_isRpdoEnabled = false; }
	
	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);

	std::vector<std::string_view> watchEntriesList() const;
	bool isConnectionOk() const { return m_isConnectionOk; }
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

	void sendRpdo();
	void handleFrame(can_frame frame);
	void checkConnection();
};



























} // namespace ucanopen


