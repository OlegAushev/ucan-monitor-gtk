/**
 * @file ucanopen_server_watch.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "ucanopen_server_watch.h"


namespace ucanopen {


///
///
///
ServerWatchService::ServerWatchService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig)
	: watchCategory(dictionaryConfig.watchCategory)
	, watchSubcategory(dictionaryConfig.watchSubcategory)
	, _server(server)
{
	_timepoint = std::chrono::steady_clock::now();

	for (const auto& [key, entry] : dictionary)
	{
		// create watch entries list and data map
		if (entry.category == watchCategory)
		{
			_entriesList.push_back(entry.name);
			_data.insert({entry.name, "..."});
		}
	}
}


}


