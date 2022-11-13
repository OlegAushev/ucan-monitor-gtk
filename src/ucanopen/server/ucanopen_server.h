/**
 * @file ucanopen_servernode.h
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
public:
	NodeId nodeId;
private:
	std::shared_ptr<can::Socket> m_socket;
private:
	/* OBJECT DICTIONARY */
	const ObjectDictionaryType& m_dictionary;
	ObjectDictionaryAuxType m_dictionaryAux;

	/* TPDO server --> client */
private:
	struct TpdoInfo
	{
		canid_t id;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoInfo;
protected:
	virtual void processTpdo1(std::array<uint8_t, 8> data) = 0;
	virtual void processTpdo2(std::array<uint8_t, 8> data) = 0;
	virtual void processTpdo3(std::array<uint8_t, 8> data) = 0;
	virtual void processTpdo4(std::array<uint8_t, 8> data) = 0;
	void registerTpdo(TpdoType type)
	{
		canid_t id = calculateCobId(toCobType(type), nodeId.value());
		m_tpdoInfo.insert({type, {id, std::chrono::steady_clock::now()}});
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
	std::map<RpdoType, RpdoInfo> m_rpdoInfo;
protected:
	virtual std::array<uint8_t, 8> makeRpdo1() = 0;
	virtual std::array<uint8_t, 8> makeRpdo2() = 0;
	virtual std::array<uint8_t, 8> makeRpdo3() = 0;
	virtual std::array<uint8_t, 8> makeRpdo4() = 0;
	void registerRpdo(RpdoType type, std::chrono::milliseconds period)
	{
		canid_t id = calculateCobId(toCobType(type), nodeId.value());
		m_rpdoInfo.insert({type, {id, period, std::chrono::steady_clock::now()}});
	}

	/* TSDO server --> client */
protected:
	virtual void processTsdo(SdoType, ObjectDictionaryType::const_iterator, CobSdoData) = 0;

public:
	IServer(NodeId t_nodeId, std::shared_ptr<can::Socket> t_socket, const ObjectDictionaryType& t_dictionary);
	void enableRpdo() { m_isRpdoEnabled = true; }
	void disableRpdo() { m_isRpdoEnabled = false; }
	void sendRpdo();

	void onFrameReceived(can_frame frame);

	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);

	std::vector<std::string_view> watchEntriesList() const;

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
};



























} // namespace ucanopen


