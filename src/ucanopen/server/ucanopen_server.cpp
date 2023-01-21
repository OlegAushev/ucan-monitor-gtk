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
	, heartbeatService(this, std::chrono::milliseconds(2000))
	, tpdoService(this)
	, rpdoService(this)
	, watchService(this, dictionary, dictionaryConfig)
	, configService(this, dictionary, dictionaryConfig)
{}


///
///
///
void Server::_setNodeId(NodeId nodeId)
{
	if (!nodeId.is_valid()) return;

	_nodeId = nodeId;

	heartbeatService.updateNodeId();
	tpdoService.updateNodeId();
	rpdoService.updateNodeId();
}


///
///
///
void Server::_send()
{	
	rpdoService.send();
	watchService.send();
}


///
///
///
void Server::_handleFrame(const can_frame& frame)
{
	if (tpdoService.handleFrame(frame))
	{
		return;
	}
	else if (frame.can_id == calculate_cob_id(CobType::tsdo, _nodeId))
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
		case cs_codes::sdo_scs_read:
			if (odEntry->second.dataType == ODEntryDataType::OD_TASK)
			{
				sdoType = SdoType::response_to_task;
			}
			else
			{
				sdoType = SdoType::response_to_read;
			}
			break;
		case cs_codes::sdo_scs_write:
			sdoType = SdoType::response_to_write;
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


