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
	const std::string_view configCategory;
private:
	impl::Server* const _server;
	std::map<std::string_view, std::vector<std::string_view>> _entriesList;
public:
	ServerConfigService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig);

	std::map<std::string_view, std::vector<std::string_view>> entriesList() const
	{
		return _entriesList;
	}
};

} // namespace ucanopen

