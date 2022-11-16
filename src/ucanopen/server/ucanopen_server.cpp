/**
 * @file ucanopen_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen_server.h"


namespace ucanopen {


///
///
///
IServer::IServer(NodeId nodeId_, std::shared_ptr<can::Socket> socket, const ObjectDictionaryType& dictionary)
	: nodeId(nodeId_)
	, m_socket(socket)
	, m_dictionary(dictionary)
{
	for (const auto& [key, entry] : m_dictionary)
	{
		m_dictionaryAux.insert({
				{entry.category, entry.subcategory, entry.name},
				m_dictionary.find(key)});
	}

	m_isRpdoEnabled = false;
}


///
///
///
void IServer::sendRpdo()
{
	auto now = std::chrono::steady_clock::now();
	
	if (!m_isRpdoEnabled) return;

	for (auto& [type, message] : m_rpdoList)
	{
		if (message.period == std::chrono::milliseconds(0)) continue;
		if (now - message.timepoint < message.period) continue;

		can_payload data;
		switch (type)
		{
		case RpdoType::RPDO1:
			data = createRpdo1();
			break;
		case RpdoType::RPDO2:
			data = createRpdo2();
			break;
		case RpdoType::RPDO3:
			data = createRpdo3();
			break;
		case RpdoType::RPDO4:
			data = createRpdo4();
			break;
		}
		m_socket->send(makeFrame(message.id, 8, data));
		message.timepoint = now;	
	}
}


///
///
///
void IServer::handleFrame(const can_frame& frame)
{
	for (auto& [type, message] : m_tpdoList)
	{
		if (frame.can_id != message.id) continue;

		message.timepoint = std::chrono::steady_clock::now();
		message.isOnSchedule = true;
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());

		switch (type)
		{
		case TpdoType::TPDO1:
			handleTpdo1(data);
			break;
		case TpdoType::TPDO2:
			handleTpdo2(data);
			break;
		case TpdoType::TPDO3:
			handleTpdo3(data);
			break;
		case TpdoType::TPDO4:
			handleTpdo4(data);
			break;
		}
		return;
	}

	if (frame.can_id == calculateCobId(CobType::TSDO, nodeId.value()))
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

		handleTsdo(type, odEntry, msg.data);
	}	
}


///
///
///
ODRequestStatus IServer::read(std::string_view category, std::string_view subcategory, std::string_view name)
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
	
	can_payload data;
	memcpy(data.begin(), &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus IServer::write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdoData)
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

	can_payload data;
	memcpy(data.begin(), &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus IServer::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
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

	can_payload data;
	memcpy(data.begin(), &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
ODRequestStatus IServer::exec(std::string_view category, std::string_view subcategory, std::string_view name)
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

	can_payload data;
	memcpy(data.begin(), &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
std::vector<std::string_view> IServer::watchEntriesList() const
{
	std::vector<std::string_view> list;
	for (auto [key, entry] : m_dictionary)
	{
		if (entry.category == "WATCH")
		{
			list.push_back(entry.name);
		}
	}
	return list;
}


///
///
///
void IServer::checkConnection()
{
	bool isConnectionOk = true;
	auto now = std::chrono::steady_clock::now();

	for (auto& [type, message] : m_tpdoList)
	{
		if (message.timeout != std::chrono::milliseconds(0)) continue;
		if ((now - message.timepoint) > message.timeout)
		{
			message.isOnSchedule = false;
			isConnectionOk = false;
		}
	}

	m_isConnectionOk = isConnectionOk;
}




} // namespace ucanopen


