/**
 * @file ucanopen_base_server.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once


#include "../ucanopen_def.h" 
#include "cansocket/cansocket.h"


namespace ucanopen {


namespace impl {


class IBaseServer
{
protected:
	std::string _name = "unnamed";
	NodeId _nodeId;
	std::shared_ptr<can::Socket> _socket;

	const ObjectDictionary& _dictionary;
	ObjectDictionaryAux _dictionaryAux;
public:
	/**
	 * @brief Construct a new IBaseServer object
	 * 
	 * @param name 
	 * @param nodeId 
	 * @param socket 
	 * @param dictionary 
	 * @param dictionaryConfig 
	 */
	IBaseServer(const std::string& name, NodeId nodeId, std::shared_ptr<can::Socket> socket,
		const ObjectDictionary& dictionary);

	/**
	 * @brief Returns server name.
	 * 
	 * @return std::string_view 
	 */
	std::string_view name() const
	{
		return _name;
	}

	/**
	 * @brief Returns server node ID.
	 * 
	 * @return NodeId 
	 */
	NodeId nodeId() const
	{
		return _nodeId;
	}

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus read(std::string_view category, std::string_view subcategory, std::string_view name);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @param data 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, CobSdoData data);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @param value 
	 * @return ODRequestStatus 
	 */
	ODRequestStatus write(std::string_view category, std::string_view subcategory, std::string_view name, std::string value);

	/**
	 * @brief 
	 * 
	 * @param category 
	 * @param subcategory 
	 * @param name 
	 * @return ODRequestStatus 
	 */
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


