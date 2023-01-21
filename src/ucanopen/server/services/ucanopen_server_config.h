#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"

#include <vector>
#include <map>


namespace ucanopen {

class ServerConfigService
{
public:
	const std::string_view config_category;
private:
	impl::Server* const _server;
	std::map<std::string_view, std::vector<std::string_view>> _entries_list;
public:
	ServerConfigService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionary_config);

	std::map<std::string_view, std::vector<std::string_view>> entries_list() const
	{
		return _entries_list;
	}
};

} // namespace ucanopen

