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
ServerNode::ServerNode(NodeId t_nodeId, std::shared_ptr<can::Socket> t_socket, const ObjectDictionaryType& t_dictionary)
	: nodeId(t_nodeId)
	, m_socket(t_socket)
	, m_dictionary(t_dictionary)
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
		if (frame.can_id == cobId(toCobType(tpdo.first), nodeId.value()))
		{
			std::array<uint8_t, 8> data;
			memcpy(&data, frame.data, frame.can_dlc);
			(void) std::async(tpdo.second.callbackOnRecv, data);
			tpdo.second.timepoint = std::chrono::steady_clock::now();
			return;
		}
	}

	if (frame.can_id == cobId(CobType::TSDO, nodeId.value()))
	{
		CobSdo msg;
		memcpy(&msg, frame.data, sizeof(CobSdo));
		ODEntryKey key = {msg.index, msg.subindex};
		auto odEntry = m_dictionary.find(key);
		if (odEntry == m_dictionary.end())
		{
			return;
		}

		SdoType type;
		switch (msg.cs)
		{
		case SDO_SCS_READ:
			if (odEntry->second.dataType == ODEntryDataType::OD_TASK)
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
		
		(void) std::async(callbackOnSdoRecv, type, odEntry, msg.data);
	}	
}


///
///
///
ODRequestStatus ServerNode::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == m_dictionary.end())
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
#endif
		return ODRequestStatus::REQUEST_FAIL;
	}
	else if (entryIt->second.hasReadAccess() == false)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
#endif
		return ODRequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_READ;
	
	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus ServerNode::write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdoData)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == m_dictionary.end())
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
#endif
		return ODRequestStatus::REQUEST_FAIL;;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
#endif
		return ODRequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_WRITE;
	message.data = sdoData;

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus ServerNode::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == m_dictionary.end())
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
#endif
		return ODRequestStatus::REQUEST_FAIL;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
#endif
		return ODRequestStatus::REQUEST_NO_ACCESS;
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
			return ODRequestStatus::REQUEST_FAIL;
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
		return ODRequestStatus::REQUEST_FAIL;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = SDO_CCS_WRITE;
	message.data = sdoData;

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus ServerNode::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	if (entryIt == m_dictionary.end())
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
#endif
		return ODRequestStatus::REQUEST_FAIL;
	}
	else if (entryIt->second.dataType != ODEntryDataType::OD_TASK)
	{
#ifdef STD_COUT_ENABLED
		std::cout << "[ucanopen] Cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry." << std::endl;
#endif
		return ODRequestStatus::REQUEST_NO_ACCESS;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;

	if (entryIt->second.hasReadAccess())
	{
		message.cs = SDO_CCS_READ;
	}
	else if (entryIt->second.hasWriteAccess())
	{
		message.cs = SDO_CCS_WRITE;
	}

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
std::vector<std::string_view> ServerNode::watchEntriesList() const
{
	std::vector<std::string_view> list;
	for (auto entry : m_dictionary)
	{
		if (entry.second.category == "WATCH")
		{
			list.push_back(entry.second.name);
		}
	}
	return list;
}


} // namespace ucanopen


