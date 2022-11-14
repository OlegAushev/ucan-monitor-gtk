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
	for (const auto& entry : m_dictionary)
	{
		m_dictionaryAux.insert({
				{entry.second.category, entry.second.subcategory, entry.second.name},
				m_dictionary.find(entry.first)});
	}

	m_isRpdoEnabled = false;
}


///
///
///
void IServer::sendRpdo()
{
	auto now = std::chrono::steady_clock::now();
	if (m_isRpdoEnabled)
	{
		for (auto& rpdo : m_rpdoInfo)
		{
			if (rpdo.second.period != std::chrono::milliseconds(0))
			{
				if (now - rpdo.second.timepoint >= rpdo.second.period)
				{
					std::array<uint8_t, 8> data;
					switch (rpdo.first)
					{
					case RpdoType::RPDO1:
						data = makeRpdo1();
						break;
					case RpdoType::RPDO2:
						data = makeRpdo2();
						break;
					case RpdoType::RPDO3:
						data = makeRpdo3();
						break;
					case RpdoType::RPDO4:
						data = makeRpdo4();
						break;
					}
					m_socket->send(makeFrame(rpdo.second.id, 8, data));
					rpdo.second.timepoint = now;
				}
			}
		}
	}
}


///
///
///
void IServer::processFrame(can_frame frame)
{
	for (auto& tpdo : m_tpdoInfo)
	{
		if (frame.can_id == tpdo.second.id)
		{
			tpdo.second.timepoint = std::chrono::steady_clock::now();
			tpdo.second.isOnSchedule = true;
			std::array<uint8_t, 8> data;
			memcpy(&data, frame.data, frame.can_dlc);

			switch (tpdo.first)
			{
			case TpdoType::TPDO1:
				processTpdo1(data);
				break;
			case TpdoType::TPDO2:
				processTpdo2(data);
				break;
			case TpdoType::TPDO3:
				processTpdo3(data);
				break;
			case TpdoType::TPDO4:
				processTpdo4(data);
				break;
			}
			return;
		}
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

		processTsdo(type, odEntry, msg.data);
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
	
	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

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

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

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

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

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

	std::array<uint8_t, 8> data;
	memcpy(&data, &message, sizeof(CobSdo));

	m_socket->send(makeFrame(CobType::RSDO, nodeId, data));
	return ODRequestStatus::REQUEST_SUCCESS;
}


///
///
///
std::vector<std::string_view> IServer::watchEntriesList() const
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


///
///
///
void IServer::checkConnection()
{
	bool isConnectionOk = true;
	auto now = std::chrono::steady_clock::now();

	for (auto& tpdo : m_tpdoInfo)
	{
		if (tpdo.second.period != std::chrono::milliseconds(0)
				&& ((now - tpdo.second.timepoint) > tpdo.second.period))
		{
			tpdo.second.isOnSchedule = false;
			isConnectionOk = false;
		}
	}

	m_isConnectionOk = isConnectionOk;
}




} // namespace ucanopen


