#include "ucanopen_server_watch.h"


namespace ucanopen {

ServerWatchService::ServerWatchService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig)
	: watchCategory(dictionaryConfig.watch_category)
	, watchSubcategory(dictionaryConfig.watch_subcategory)
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

