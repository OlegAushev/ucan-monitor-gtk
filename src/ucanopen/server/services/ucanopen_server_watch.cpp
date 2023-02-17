#include "ucanopen_server_watch.h"


namespace ucanopen {

ServerWatchService::ServerWatchService(impl::Server* server, impl::SdoPublisher* sdo_publisher)
	: impl::SdoSubscriber(sdo_publisher)
	, _server(server)
{
	_timepoint = std::chrono::steady_clock::now();

	for (const auto& [key, object] : _server->dictionary().entries)
	{
		// create watch entries list and data map
		if (object.category == _server->dictionary().config.watch_category)
		{
			_entries_list.push_back(object.name);
			_data.insert({object.name, "..."});
		}
	}
}

}

