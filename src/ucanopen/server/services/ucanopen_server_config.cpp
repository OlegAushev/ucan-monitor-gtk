#include "ucanopen_server_config.h"


namespace ucanopen {

ServerConfigService::ServerConfigService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionary_config)
	: config_category(dictionary_config.config_category)
	, _server(server)
{
	for (const auto& [key, entry] : dictionary)
	{
		// create conf entries list
		if (entry.category == config_category)
		{
			_entries_list[entry.subcategory].push_back(entry.name);
		}
	}
}

} // namespace ucanopen

