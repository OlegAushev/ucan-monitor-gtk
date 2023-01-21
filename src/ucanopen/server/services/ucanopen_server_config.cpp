#include "ucanopen_server_config.h"


namespace ucanopen {

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

} // namespace ucanopen

