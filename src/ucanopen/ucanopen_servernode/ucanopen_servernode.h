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


class ServerNode
{
public:
	enum class Name
	{
		C2000,
	};

	NodeId nodeId;
private:
	std::shared_ptr<can::Socket> m_socket;
private:
	/* OBJECT DICTIONARY */
	const ObjectDictionaryType& m_dictionary;
	ObjectDictionaryAuxType m_dictionaryAux;
private:
	/* TPDO server --> client */
	struct TpdoInfo
	{
		std::function<void(std::array<uint8_t, 8>)> callbackOnRecv;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<TpdoType, TpdoInfo> m_tpdoInfo;

	/* RPDO server <-- client */
	bool m_isRpdoEnabled;
	struct RpdoInfo
	{
		std::function<std::array<uint8_t, 8>(void)> callbackOnSend;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
	};
	std::map<RpdoType, RpdoInfo> m_rpdoInfo;

	/* SDO */
	std::function<void(SdoType, std::map<ODEntryKey,ODEntryValue>::const_iterator, CobSdoData)> m_rsdoCallback;

public:
	ServerNode(NodeId t_nodeId, std::shared_ptr<can::Socket> t_socket, const ObjectDictionaryType& t_dictionary);

public:
	void registerCallbackOnRecvPdo(TpdoType tpdoType, std::function<void(std::array<uint8_t, 8>)> callback)
	{
		m_tpdoInfo[tpdoType].callbackOnRecv = callback;
	}

	void registerCallbackOnSendPdo(RpdoType rpdoType, std::function<std::array<uint8_t, 8>(void)> callback, std::chrono::milliseconds period)
	{
		m_rpdoInfo[rpdoType].callbackOnSend = callback;
		m_rpdoInfo[rpdoType].period = period;
	}

public:
	void enableRpdo() { m_isRpdoEnabled = true; }
	void disableRpdo() { m_isRpdoEnabled = false; }
	void sendRpdo();
	void onFrameReceived(can_frame frame);

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
public:
	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);

public:
	std::set<std::string> watchEntriesList() const;
};


} // namespace ucanopen


