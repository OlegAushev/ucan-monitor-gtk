/**
 * @file ucanopen_impl_server.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_impl_server.h"


namespace ucanopen {


///
///
///
impl::Server::Server(const std::string& name, NodeId nodeId, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary)
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
	}
}


///
///
///
ODRequestStatus impl::Server::read(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = _findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;
	}
	else if (entryIt->second.hasReadPermission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot read "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::no_access;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdo_ccs_read;

	_socket->send(create_frame(CobType::rsdo, _nodeId, message.to_payload()));
	return ODRequestStatus::success;
}


///
///
///
ODRequestStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData sdoData)
{
	auto entryIt = _findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;;
	}
	else if (entryIt->second.hasWritePermission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::no_access;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdo_ccs_write;
	message.data = sdoData;

	_socket->send(create_frame(CobType::rsdo, _nodeId, message.to_payload()));
	return ODRequestStatus::success;
}


///
///
///
ODRequestStatus impl::Server::write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value)
{
	auto entryIt = _findOdEntry(category, subcategory, name);
	
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;
	}
	else if (entryIt->second.hasWritePermission() == false)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot write "
				<< category << "::" << subcategory << "::" << name 
				<< " - no access." << std::endl;
		return ODRequestStatus::no_access;
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
			return ODRequestStatus::fail;
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
		return ODRequestStatus::fail;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;
	message.cs = cs_codes::sdo_ccs_write;
	message.data = sdoData;

	_socket->send(create_frame(CobType::rsdo, _nodeId, message.to_payload()));
	return ODRequestStatus::success;
}


///
///
///
ODRequestStatus impl::Server::exec(std::string_view category, std::string_view subcategory, std::string_view name)
{
	auto entryIt = _findOdEntry(category, subcategory, name);
	if (entryIt == _dictionary.end())
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - no such OD entry." << std::endl;
		return ODRequestStatus::fail;
	}
	else if (entryIt->second.dataType != ODEntryDataType::OD_TASK)
	{
		std::cout << "[ucanopen] '" << _name << "' server: cannot execute "
				<< category << "::" << subcategory << "::" << name 
				<< " - not executable OD entry." << std::endl;
		return ODRequestStatus::no_access;
	}

	CobSdo message{};
	message.index = entryIt->first.index;
	message.subindex = entryIt->first.subindex;

	if (entryIt->second.hasReadPermission())
	{
		message.cs = cs_codes::sdo_ccs_read;
	}
	else if (entryIt->second.hasWritePermission())
	{
		message.cs = cs_codes::sdo_ccs_write;
	}

	_socket->send(create_frame(CobType::rsdo, _nodeId, message.to_payload()));
	return ODRequestStatus::success;
}


}


