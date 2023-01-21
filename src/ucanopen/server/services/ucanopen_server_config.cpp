/**
 * @file ucanopen_server_config.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_server_config.h"


namespace ucanopen {


///
///
///
ServerConfigService::ServerConfigService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig)
	: configCategory(dictionaryConfig.config_category)
	, _server(server)
{
	for (const auto& [key, entry] : dictionary)
	{
		// create conf entries list
		if (entry.category == configCategory)
		{
			_entriesList[entry.subcategory].push_back(entry.name);
		}
	}
}


}


