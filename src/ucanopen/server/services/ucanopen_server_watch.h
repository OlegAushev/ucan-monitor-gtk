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


#include "../../ucanopen_def.h" 
#include "cansocket/cansocket.h"

#include <vector>
#include <map>
#include <mutex>


namespace ucanopen {


class ServerWatchService
{
public:
	const std::string_view dictionaryCategory;
	const std::string_view dictionarySubcategory;
private:
	std::string _name = "foo"; // FIXME
	bool _isEnabled = false;
	std::chrono::milliseconds _period = std::chrono::milliseconds(1000);
	std::chrono::time_point<std::chrono::steady_clock> _timepoint;
	std::vector<std::string_view> _entriesList;
	mutable std::mutex _mutex;
	std::map<std::string_view, std::string> _data;
public:
	ServerWatchService(const ObjectDictionary& dictionary, const ObjectDictionaryConfig& dictionaryConfig);


	void sendPeriodicRequest()
	{
		if (_isEnabled && !_entriesList.empty())
		{
			auto now = std::chrono::steady_clock::now();
			if (now - _timepoint >= _period)
			{
				static size_t i = 0;
				// TODO read(dictionaryCategory, dictionarySubcategory, _entriesList[i]);
				_timepoint = now;
				i = (i + 1) % _entriesList.size();
			}
		}
	}

	bool handleFrame(SdoType sdoType, ObjectDictionary::const_iterator odEntry, CobSdoData data)
	{
		if ((odEntry->second.category == dictionaryCategory) && (sdoType == SdoType::ResponseToRead))
		{
			if (odEntry->second.dataType != OD_ENUM16)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_data[odEntry->second.name] = data.toString(odEntry->second.dataType);
			}
			return true;
		}
		return false;
	}

	/**
	 * @brief 
	 * 
	 */
	void enable()
	{
		std::cout << "[ucanopen] Enabling '" << _name << "' server watch requests (period = " << _period << ")... ";
		_isEnabled = true;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 */
	void disable()
	{
		std::cout << "[ucanopen] Disabling '" << _name << "' server watch requests... ";
		_isEnabled = false;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief
	 * 
	 * @param period 
	 */
	void setPeriod(std::chrono::milliseconds period)
	{
		std::cout << "[ucanopen] Setting '" << _name << "' server watch requests period = " << period << "... ";
		_period = period;
		std::cout << "done." << std::endl;
	}

	/**
	 * @brief 
	 * 
	 * @return std::vector<std::string_view> 
	 */
	std::vector<std::string_view> entriesList() const
	{
		return _entriesList;
	}

	/**
	 * @brief 
	 * 
	 * @param watchName 
	 * @return std::string 
	 */
	std::string value(std::string_view watchName) const
	{
		auto it = _data.find(watchName);
		if (it == _data.end())
		{
			return "n/a";
		}
		return it->second;
	}

	/**
	 * @brief 
	 * 
	 * @param watchName 
	 * @param buf 
	 * @param len 
	 */
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


