/**
 * @file ucanopen_servernode.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen_servernode.h"


namespace ucanopen {


///
///
///
ServerNode::ServerNode(NodeId _nodeId, std::shared_ptr<can::Socket> _socket, const ObjectDictionaryType& _dictionary)
	: nodeId(_nodeId)
	, m_socket(_socket)
	, m_dictionary(_dictionary)
{
	for (const auto& entry : m_dictionary)
	{
		m_dictionaryAux.insert({
				{entry.second.category, entry.second.subcategory, entry.second.name},
				m_dictionary.find(entry.first)});
	}

	m_isRpdoEnabled = false;

	m_tpdoInfo.insert({TpdoType::TPDO1, {{}, std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO2, {{}, std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO3, {{}, std::chrono::steady_clock::now()}});
	m_tpdoInfo.insert({TpdoType::TPDO4, {{}, std::chrono::steady_clock::now()}});

	m_rpdoInfo.insert({RpdoType::RPDO1, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_rpdoInfo.insert({RpdoType::RPDO2, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_rpdoInfo.insert({RpdoType::RPDO3, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
	m_rpdoInfo.insert({RpdoType::RPDO4, {{}, std::chrono::milliseconds(0), std::chrono::steady_clock::now()}});
}


///
///
///
void ServerNode::sendRpdo()
{
	auto now = std::chrono::steady_clock::now();
	if (m_isRpdoEnabled)
	{
		for (auto& rpdo : m_rpdoInfo)
		{
			if (rpdo.second.callbackOnSend)
			{
				if (now - rpdo.second.timepoint >= rpdo.second.period)
				{
					m_socket->send(makeFrame(toCobType(rpdo.first), nodeId, rpdo.second.callbackOnSend()));
					rpdo.second.timepoint = now;
				}
			}
		}
	}
}


///
///
///
void ServerNode::onFrameReceived(can_frame frame)
{
	for (auto& tpdo : m_tpdoInfo)
	{
		if (frame.can_id == cobId(toCobType(tpdo.first), nodeId.value))
		{
			std::array<uint8_t, 8> data;
			memcpy(&data, frame.data, frame.can_dlc);
			(void) std::async(tpdo.second.callbackOnRecv, data);
			tpdo.second.timepoint = std::chrono::steady_clock::now();
			return;
		}
	}

	if (frame.can_id == cobId(CobType::TSDO, nodeId.value))
	{
		CobSdo msg;
		memcpy(&msg, frame.data, sizeof(CobSdo));
		OD_EntryKey key = {msg.index, msg.subindex};
		auto odEntry = m_dictionary.find(key);
		if (odEntry == m_dictionary.end())
		{
			return;
		}

		SdoType type;
		switch (msg.cs)
		{
		case SDO_SCS_READ:
			if (odEntry->second.dataType == OD_EntryDataType::OD_TASK)
			{
				type = SdoType::RESPONSE_TO_TASK;
			}
			else
			{
				type = SdoType::RESPONSE_TO_READ;
			}
			break;
		case SDO_SCS_WRITE:
			type = SdoType::RESPONSE_TO_WRITE;
			break;
		default:
			return;
		}
		
		(void) std::async(m_rsdoCallback, type, odEntry, msg.data);
	}	
}


///
///
///
OD_RequestStatus ServerNode::read(const std::string& category, const std::string& subcategory, const std::string& name)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == m_dictionary.end())
	{
		return OD_RequestStatus::REQUEST_FAIL;
	}
	else if (entryIt->second.hasReadAccess() == false)
	{
		return OD_RequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdo message = {};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_READ;
	
	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return OD_RequestStatus::REQUEST_SUCCESS;
}


///
///
///
OD_RequestStatus ServerNode::exec(const std::string& odEntryCategory,
		const std::string& odEntrySubcategory,
		const std::string& odEntryName)
{
	return read(odEntryCategory, odEntrySubcategory, odEntryName);
}


///
///
///
OD_RequestStatus ServerNode::write(const std::string& category, const std::string& subcategory, const std::string& name, CobSdoData sdoData)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == m_dictionary.end())
	{
		return OD_RequestStatus::REQUEST_FAIL;;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
		return OD_RequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdo message = {};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_WRITE;
	message.data = sdoData;

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return OD_RequestStatus::REQUEST_SUCCESS;
}


///
///
///
OD_RequestStatus ServerNode::write(
		const std::string& odEntryCategory,
		const std::string& odEntrySubcategory,
		const std::string& odEntryName,
		const std::string& value)
{
	auto entryIt = findOdEntry(odEntryCategory, odEntrySubcategory, odEntryName);
	
	if (entryIt == m_dictionary.end())
	{
		return OD_RequestStatus::REQUEST_FAIL;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
		return OD_RequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdoData sdoData;

	switch (entryIt->second.dataType)
	{
	case OD_BOOL:
		if (value == "TRUE" || value == "ON" || value == "1")
			sdoData = CobSdoData(true);
		else if (value == "FALSE" || value == "OFF" || value == "0")
			sdoData = CobSdoData(true);
		else
			return OD_RequestStatus::REQUEST_FAIL;
		break;
	case OD_INT16:
		sdoData = CobSdoData(int16_t(std::stoi(value)));
		break;
	case OD_INT32:
		sdoData = CobSdoData(int32_t(std::stoi(value)));
		break;
	case OD_UINT16:
		sdoData = CobSdoData(uint16_t(std::stoul(value)));
		break;
	case OD_UINT32:
		sdoData = CobSdoData(uint32_t(std::stoul(value)));
		break;
	case OD_FLOAT32:
		sdoData = CobSdoData(float(std::stof(value)));
		break;
	case OD_ENUM16:
		sdoData = CobSdoData(uint16_t(std::stoi(value)));
		break;
	default:
		return OD_RequestStatus::REQUEST_FAIL;
	}

	CobSdo message = {};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_WRITE;
	message.data = sdoData;

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return OD_RequestStatus::REQUEST_SUCCESS;
}


///
///
///
std::set<std::string> ServerNode::watchEntriesList()
{
	std::set<std::string> list;
	for (auto entry : m_dictionary)
	{
		if ((entry.second.category == "WATCH") 
				&& (entry.second.dataType == OD_FLOAT32))
		{
			list.insert(entry.second.name);
		}
	}
	return list;
}


} // namespace ucanopen


