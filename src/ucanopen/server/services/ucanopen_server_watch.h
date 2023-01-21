/**
 * @file ucanopen_server_watch.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */


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
	const std::string_view watchCategory;
	const std::string_view watchSubcategory;
private:
	impl::Server* const _server;
	bool _isEnabled = false;
	std::chrono::milliseconds _period = std::chrono::milliseconds(1000);
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
	std::vector<std::string_view> _entriesList;
	mutable std::mutex _mutex;
	std::map<std::string_view, std::string> _data;
public:
	ServerWatchService(impl::Server* server, const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig);

	void send()
	{
		if (_isEnabled && !_entriesList.empty())
		{
			auto now = std::chrono::steady_clock::now();
			if (now - _timepoint >= _period)
			{
				static size_t i = 0;
				_server->read(watchCategory, watchSubcategory, _entriesList[i]);
				_timepoint = now;
				i = (i + 1) % _entriesList.size();
			}
		}
	}

	bool handleFrame(SdoType sdoType, ObjectDictionary::const_iterator odEntry, CobSdoData data)
	{
		if ((odEntry->second.category == watchCategory) && (sdoType == SdoType::response_to_read))
		{
			if (odEntry->second.dataType != OD_ENUM16)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_data[odEntry->second.name] = data.to_string(odEntry->second.dataType);
			}
			return true;
		}
		return false;
	}

	void enable()
	{
		std::cout << "[ucanopen] Enabling '" << _server->name() << "' server watch requests (period = " << _period << ")... ";
		_isEnabled = true;
		std::cout << "done." << std::endl;
	}

	void disable()
	{
		std::cout << "[ucanopen] Disabling '" << _server->name() << "' server watch requests... ";
		_isEnabled = false;
		std::cout << "done." << std::endl;
	}

	void setPeriod(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting '" << _server->name() << "' server watch requests period = " << period << "... ";
		_period = period;
		std::cout << "done." << std::endl;
	}

	std::vector<std::string_view> entriesList() const
	{
		return _entriesList;
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

	void setValue(std::string_view watchName, std::string value)
	{
		if (!_data.contains(watchName)) return;
		_data[watchName] = value;
	}
};


}


