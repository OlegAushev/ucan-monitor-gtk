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
	ServerNode(NodeId _nodeId, std::shared_ptr<can::Socket> _socket, const ObjectDictionaryType& _dictionary);

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
		const std::string& category,
		const std::string& subcategory,
		const std::string& name)
	{
		auto it = m_dictionaryAux.find({category, subcategory, name});
		if (it == m_dictionaryAux.end())
		{
			return m_dictionary.end();
		}
		return it->second;
	}
public:
	ODRequestStatus read(const std::string& category, const std::string& subcategory, const std::string& name);
	ODRequestStatus exec(const std::string& category, const std::string& subcategory, const std::string& name);
	ODRequestStatus write(const std::string& category, const std::string& subcategory, const std::string& name, CobSdoData data = {});
	ODRequestStatus write(const std::string& category, const std::string& subcategory, const std::string& name, const std::string& value);

public:
	std::set<std::string> watchEntriesList();
};


} // namespace ucanopen


