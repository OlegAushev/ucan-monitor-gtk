#include "ucanopen_server_watch.h"


namespace ucanopen {

ServerWatchService::ServerWatchService(impl::Server* server)
	: _server(server)
{
	_timepoint = std::chrono::steady_clock::now();

	for (const auto& [key, entry] : _server->dictionary().entries)
	{
		// create watch entries list and data map
		if (entry.category == _server->dictionary().config.watch_category)
		{
			_entries_list.push_back(entry.name);
			_data.insert({entry.name, "..."});
		}
	}
}

}

