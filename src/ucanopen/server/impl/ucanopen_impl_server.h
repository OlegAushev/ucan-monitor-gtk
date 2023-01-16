/**
 * @file ucanopen_impl_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 


namespace ucanopen {


class ServerHeartbeatService;
class ServerTpdoService;
class ServerRpdoService;


namespace impl {


class Server
{
	friend class ucanopen::ServerHeartbeatService;
	friend class ucanopen::ServerTpdoService;
	friend class ucanopen::ServerRpdoService;
protected:
	std::string _name = "unnamed";
	NodeId _nodeId;
	std::shared_ptr<can::Socket> _socket;

	const ObjectDictionary& _dictionary;
	ObjectDictionaryAux _dictionaryAux;

	NmtState _nmtState = NmtState::Stopped;
protected:
	virtual can_payload _createRpdo1() = 0;
	virtual can_payload _createRpdo2() = 0;
	virtual can_payload _createRpdo3() = 0;
	virtual can_payload _createRpdo4() = 0;

	virtual void _handleTsdo(SdoType, ObjectDictionary::const_iterator, CobSdoData) = 0;	
public:
	Server(const std::string& name, NodeId nodeId, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary);

	std::string_view name() const { return _name; }
	NodeId nodeId() const {	return _nodeId; }
	NmtState nmtState() const { return _nmtState; }

	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);
	ODRequestStatus exec(std::string_view category, std::string_view subcategory, std::string_view name);
protected:
	std::map<ODEntryKey, ODEntryValue>::const_iterator _findOdEntry(
		std::string_view category,
		std::string_view subcategory,
		std::string_view name)
	{
		auto it = _dictionaryAux.find({category, subcategory, name});
		if (it == _dictionaryAux.end())
		{
			return _dictionary.end();
		}
		return it->second;
	}
};


}


}


