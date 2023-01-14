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
Server::Server(const std::string& name, NodeId nodeId, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig)
	: impl::Server(name, nodeId, socket, dictionary)
	, heartbeatService(nodeId, std::chrono::milliseconds(2000))
	, watchService(this, dictionary, dictionaryConfig)
	, configCategory(dictionaryConfig.configCategory)
{
	for (const auto& [key, entry] : _dictionary)
	{
		// create conf entries list
		if (entry.category == configCategory)
		{
			_configEntriesList[entry.subcategory].push_back(entry.name);
		}
	}
}


///
///
///
void Server::_setNodeId(NodeId nodeId)
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

	heartbeatService.setNodeId(_nodeId);
}


///
///
///
void Server::_sendPeriodic()
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
				data = _createRpdo1();
				break;
			case RpdoType::Rpdo2:
				data = _createRpdo2();
				break;
			case RpdoType::Rpdo3:
				data = _createRpdo3();
				break;
			case RpdoType::Rpdo4:
				data = _createRpdo4();
				break;
			}
			_socket->send(createFrame(message.id, 8, data));
			message.timepoint = now;	
		}
	}

	watchService.sendPeriodicRequest();
}


///
///
///
void Server::_handleFrame(const can_frame& frame)
{
	for (auto& [type, message] : _tpdoList)
	{
		if (frame.can_id != message.id) continue;

		message.timepoint = std::chrono::steady_clock::now();
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
		message.data = data;

		switch (type)
		{
		case TpdoType::Tpdo1:
			_handleTpdo1(data);
			break;
		case TpdoType::Tpdo2:
			_handleTpdo2(data);
			break;
		case TpdoType::Tpdo3:
			_handleTpdo3(data);
			break;
		case TpdoType::Tpdo4:
			_handleTpdo4(data);
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
		watchService.handleFrame(sdoType, odEntry, sdoMessage.data);

		// server-specific TSDO handling
		_handleTsdo(sdoType, odEntry, sdoMessage.data);
	}
	else if (heartbeatService.handleFrame(frame))
	{
		return;
	}	
}


} // namespace ucanopen


