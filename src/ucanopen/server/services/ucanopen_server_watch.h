#pragma once


#include "../impl/ucanopen_impl_server.h"
#include <vector>
#include <map>
#include <mutex>


namespace ucanopen {

class ServerWatchService : public impl::SdoSubscriber
{
private:
	impl::Server* const _server;
	bool _is_enabled = false;
	std::chrono::milliseconds _period = std::chrono::milliseconds(1000);
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
	std::vector<std::string_view> _entries_list;
	mutable std::mutex _data_access_mutex;
	std::map<std::string_view, std::string> _data;
public:
	ServerWatchService(impl::Server* server, impl::SdoPublisher* sdo_publisher);

	void send()
	{
		if (_is_enabled && !_entries_list.empty())
		{
			auto now = std::chrono::steady_clock::now();
			if (now - _timepoint >= _period)
			{
				static size_t i = 0;
				_server->read(_server->dictionary().config.watch_category, _server->dictionary().config.watch_subcategory, _entries_list[i]);
				_timepoint = now;
				i = (i + 1) % _entries_list.size();
			}
		}
	}

	virtual int handle_sdo(SdoType sdo_type, ODEntryIter entry_iter, ExpeditedSdoData sdo_data)
	{
		if ((entry_iter->second.category == _server->dictionary().config.watch_category) && (sdo_type == SdoType::response_to_read))
		{
			if (entry_iter->second.data_type != OD_ENUM16)
			{
				std::lock_guard<std::mutex> lock(_data_access_mutex);
				_data[entry_iter->second.name] = sdo_data.to_string(entry_iter->second.data_type);
			}
			return 0;
		}
		return 1;
	}

	void enable()
	{
		Log() << "[ucanopen] Enabling '" << _server->name() << "' server watch requests (period = " << _period << ")... ";
		_is_enabled = true;
		Log() << "done.\n";
	}

	void disable()
	{
		Log() << "[ucanopen] Disabling '" << _server->name() << "' server watch requests... ";
		_is_enabled = false;
		Log() << "done.\n";
	}

	void set_period(std::chrono::milliseconds period)
	{
		Log() << "[ucanopen] Setting '" << _server->name() << "' server watch requests period = " << period << "... ";
		_period = period;
		Log() << "done.\n";
	}

	std::vector<std::string_view> entries_list() const
	{
		return _entries_list;
	}

	std::string value(std::string_view watch_name) const
	{
		auto it = _data.find(watch_name);
		if (it == _data.end())
		{
			return "n/a";
		}
		return it->second;
	}

	void value(std::string_view watch_name, char* retbuf, size_t bufsize) const
	{
		retbuf[0] = '\0';
		auto it = _data.find(watch_name);
		if (it == _data.end())
		{
			const char* str = "n/a";
			std::strncat(retbuf, str, bufsize-1);
			return;
		}
		std::lock_guard<std::mutex> lock(_data_access_mutex);
		std::strncat(retbuf, it->second.c_str(), bufsize-1);
	}

	void set_value(std::string_view watch_name, std::string val)
	{
		if (!_data.contains(watch_name)) return;
		std::lock_guard<std::mutex> lock(_data_access_mutex);
		_data[watch_name] = val;
	}
};

} // namespace ucanopen

