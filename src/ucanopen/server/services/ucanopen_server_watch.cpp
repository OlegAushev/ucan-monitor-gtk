#include "ucanopen_server_watch.h"


namespace ucanopen {

ServerWatchService::ServerWatchService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionary_config)
	: watch_category(dictionary_config.watch_category)
	, watch_subcategory(dictionary_config.watch_subcategory)
	, _server(server)
{
	_timepoint = std::chrono::steady_clock::now();

	for (const auto& [key, entry] : dictionary)
	{
		// create watch entries list and data map
		if (entry.category == watch_category)
		{
			_entries_list.push_back(entry.name);
			_data.insert({entry.name, "..."});
		}
	}
}

}

