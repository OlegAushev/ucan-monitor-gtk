#pragma once


#include "cansocket/cansocket.h"
#include "../../ucanopen_def.h" 
#include "../impl/ucanopen_impl_server.h"

#include <vector>
#include <map>
#include <mutex>


namespace ucanopen {

class ServerWatchService
{
public:
	const std::string_view watch_category;
	const std::string_view watch_subcategory;
private:
	impl::Server* const _server;
	bool _is_enabled = false;
	std::chrono::milliseconds _period = std::chrono::milliseconds(1000);
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
	std::vector<std::string_view> _entries_list;
	mutable std::mutex _mutex;
	std::map<std::string_view, std::string> _data;
public:
	ServerWatchService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionary_config);

	void send()
	{
		if (_is_enabled && !_entries_list.empty())
		{
			auto now = std::chrono::steady_clock::now();
			if (now - _timepoint >= _period)
			{
				static size_t i = 0;
				_server->read(watch_category, watch_subcategory, _entries_list[i]);
				_timepoint = now;
				i = (i + 1) % _entries_list.size();
			}
		}
	}

	bool handle_frame(SdoType sdo_type, ObjectDictionary::const_iterator od_entry, CobSdoData sdo_data)
	{
		if ((od_entry->second.category == watch_category) && (sdo_type == SdoType::response_to_read))
		{
			if (od_entry->second.data_type != OD_ENUM16)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_data[od_entry->second.name] = sdo_data.to_string(od_entry->second.data_type);
			}
			return true;
		}
		return false;
	}

	void enable()
	{
		std::cout << "[ucanopen] Enabling '" << _server->name() << "' server watch requests (period = " << _period << ")... ";
		_is_enabled = true;
		std::cout << "done." << std::endl;
	}

	void disable()
	{
		std::cout << "[ucanopen] Disabling '" << _server->name() << "' server watch requests... ";
		_is_enabled = false;
		std::cout << "done." << std::endl;
	}

	void set_period(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting '" << _server->name() << "' server watch requests period = " << period << "... ";
		_period = period;
		std::cout << "done." << std::endl;
	}

	std::vector<std::string_view> entries_list() const
	{
		return _entries_list;
	}

	std::string value(std::string_view watchName) const
	{
		auto it = _data.find(watchName);
		if (it == _data.end())
		{
			return "n/a";
		}
		return it->second;
	}

	void value(std::string_view watchName, char* buf, size_t len) const
	{
		auto it = _data.find(watchName);
		if (it == _data.end())
		{
			const char* str = "n/a";
			std::strncpy(buf, str, len);
			return;
		}
		std::lock_guard<std::mutex> lock(_mutex);
		std::strncpy(buf, it->second.c_str(), len);
	}

	void set_value(std::string_view watchName, std::string value)
	{
		if (!_data.contains(watchName)) return;
		_data[watchName] = value;
	}
};

} // namespace ucanopen

