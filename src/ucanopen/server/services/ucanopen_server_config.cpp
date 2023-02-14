#include "ucanopen_server_config.h"


namespace ucanopen {

ServerConfigService::ServerConfigService(impl::Server* server)
	: _server(server)
{
	for (const auto& [key, entry] : server->dictionary().entries)
	{
		// create conf entries list
		if (entry.category == server->dictionary().config.config_category)
		{
			_entries_list[entry.subcategory].push_back(entry.name);
		}
	}
}

} // namespace ucanopen

