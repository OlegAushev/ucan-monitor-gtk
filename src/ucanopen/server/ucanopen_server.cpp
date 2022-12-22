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
IServer::IServer(const std::string& name, NodeId nodeId, std::shared_ptr<can::Socket> socket, const ObjectDictionaryType& dictionary)
	: _name(name)
	, _nodeId(nodeId)
	, _socket(socket)
	, _dictionary(dictionary)
{
	for (const auto& [key, entry] : _dictionary)
	{
		// create aux dictionary for faster search by {category, subcategory, name}
		_dictionaryAux.insert({
				{entry.category, entry.subcategory, entry.name},
				_dictionary.find(key)});

		// create watch entries list and data map
		if (entry.category == watchCategory)
		{
			_watchEntriesList.push_back(entry.name);
			_watchData.insert({entry.name, "..."});
		}

		// create conf entries list
		if (entry.category == confCategory)
		{
			_confEntriesList[entry.subcategory].push_back(entry.name);
		}
	}

	_heartbeatInfo = {
		.id = calculateCobId(CobType::Heartbeat, _nodeId),
		.timeout = std::chrono::milliseconds(2000),
		.timepoint = std::chrono::steady_clock::now(),
		.nmtState = NmtState::Stopped
	};
}


///
///
///
void IServer::setNodeId(NodeId nodeId)
{
	if (!nodeId.isValid()) return;

	_nodeId = nodeId;

	for (auto& [type, message] : _rpdoList)
	{
		message.id = calculateCobId(toCobType(type), _nodeId);
	}

	for (auto& [type, message] : _tpdoList)
	{
		message.id = calculateCobId(toCobType(type), _nodeId);
	}

	_heartbeatInfo.id = calculateCobId(CobType::Heartbeat, _nodeId);	
}


///
///
///
void IServer::sendPeriodic()
{
	auto now = std::chrono::steady_clock::now();
	
	if (_isRpdoEnabled)
	{
		for (auto& [type, message] : _rpdoList)
		{
			if (message.period == std::chrono::milliseconds(0)) continue;
			if (now - message.timepoint < message.period) continue;

			can_payload data;
			switch (type)
			{
			case RpdoType::Rpdo1:
				data = createRpdo1();
				break;
			case RpdoType::Rpdo2:
				data = createRpdo2();
				break;
			case RpdoType::Rpdo3:
				data = createRpdo3();
				break;
			case RpdoType::Rpdo4:
				data = createRpdo4();
				break;
			}
			_socket->send(createFrame(message.id, 8, data));
			message.timepoint = now;	
		}
	}

	if (_isWatchEnabled)
	{
		if (now - _watchInfo.timepoint >= _watchInfo.period)
		{
			static size_t i = 0;
			read(watchCategory, watchSubcategory, _watchEntriesList[i]);
			_watchInfo.timepoint = now;
			i = (i + 1) % _watchEntriesList.size();
		}
	}
}


///
///
///
void IServer::handleFrame(const can_frame& frame)
{
	for (auto& [type, message] : _tpdoList)
	{
		if (frame.can_id != message.id) continue;

		message.timepoint = std::chrono::steady_clock::now();
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());

		switch (type)
		{
		case TpdoType::Tpdo1:
			handleTpdo1(data);
			break;
		case TpdoType::Tpdo2:
			handleTpdo2(data);
			break;
		case TpdoType::Tpdo3:
			handleTpdo3(data);
			break;
		case TpdoType::Tpdo4:
			handleTpdo4(data);
			break;
		}
		return;
	}

	if (frame.can_id == calculateCobId(CobType::Tsdo, _nodeId))
	{
		CobSdo sdoMessage(frame.data);
		ODEntryKey key = {sdoMessage.index, sdoMessage.subindex};
		auto odEntry = _dictionary.find(key);
		if (odEntry == _dictionary.end())
		{
			return;
		}

		SdoType sdoType;
		switch (sdoMessage.cs)
		{
		case cs_codes::sdoScsRead:
			if (odEntry->second.dataType == ODEntryDataType::OD_TASK)
			{
				sdoType = SdoType::ResponseToTask;
			}
			else
			{
				sdoType = SdoType::ResponseToRead;
			}
			break;
		case cs_codes::sdoScsWrite:
			sdoType = SdoType::ResponseToWrite;
			break;
		default:
			return;
		}
		
		// handle watch data
		if ((odEntry->second.category == watchCategory) && (sdoType == SdoType::ResponseToRead))
		{
			if (odEntry->second.dataType != OD_ENUM16)
			{
				std::lock_guard<std::mutex> lock(_watchMutex);
				_watchData[odEntry->second.name] = sdoMessage.data.toString(odEntry->second.dataType);
			}
		}

		// server-specific TSDO handling
		handleTsdo(sdoType, odEntry, sdoMessage.data);
	}
	else if (frame.can_id == _heartbeatInfo.id)
	{
		_heartbeatInfo.timepoint = std::chrono::steady_clock::now();
		_heartbeatInfo.nmtState = static_cast<NmtState>(frame.data[0]);
	}	
}


///
///
///
ODRequestStatus IServer::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::Fail;
	}
	else if (entryIt->second.hasReadAccess() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::NoAccess;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdoCcsRead;

	_socket->send(createFrame(CobType::Rsdo, _nodeId, message.toPayload()));
	return ODRequestStatus::Success;
}


///
///
///
ODRequestStatus IServer::write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdoData)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::Fail;;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::NoAccess;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdoCcsWrite;
	message.data = sdoData;

	_socket->send(createFrame(CobType::Rsdo, _nodeId, message.toPayload()));
	return ODRequestStatus::Success;
}


///
///
///
ODRequestStatus IServer::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::Fail;
	}
	else if (entryIt->second.hasWriteAccess() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::NoAccess;
	}

	CobSdoData sdoData;

	switch (entryIt->second.dataType)
	{
	case OD_BOOL:
		if (value == "TRUE" || value == "true" || value == "ON" || value == "on" || value == "1")
			sdoData = CobSdoData(true);
		else if (value == "FALSE" || value == "false" || value == "OFF" || value == "off" || value == "0")
			sdoData = CobSdoData(true);
		else
			return ODRequestStatus::Fail;
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
		return ODRequestStatus::Fail;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdoCcsWrite;
	message.data = sdoData;

	_socket->send(createFrame(CobType::Rsdo, _nodeId, message.toPayload()));
	return ODRequestStatus::Success;
}


///
///
///
ODRequestStatus IServer::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = findOdEntry(category, subcategory, name);
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::Fail;
	}
	else if (entryIt->second.dataType != ODEntryDataType::OD_TASK)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry." << std::endl;
		return ODRequestStatus::NoAccess;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;

	if (entryIt->second.hasReadAccess())
	{
		message.cs = cs_codes::sdoCcsRead;
	}
	else if (entryIt->second.hasWriteAccess())
	{
		message.cs = cs_codes::sdoCcsWrite;
	}

	_socket->send(createFrame(CobType::Rsdo, _nodeId, message.toPayload()));
	return ODRequestStatus::Success;
}


} // namespace ucanopen


