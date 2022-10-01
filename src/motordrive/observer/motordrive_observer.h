/**
 * @file motordrive_observer.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/ucanopen_client.h"


namespace motordrive {



class Observer
{
private:
	std::shared_ptr<ucanopen::Client> m_ucanClient;

	std::map<std::string_view, std::string> m_watchData;
	mutable std::mutex m_watchMutex;


public:
	Observer(std::shared_ptr<ucanopen::Client> ucanClient);
	
	std::string watchValue(std::string_view watchName) const
	{
		auto it = m_watchData.find(watchName);
		if (it == m_watchData.end())
		{
			return "n/a";
		}
		return it->second;
	}

	void watchValue(std::string_view watchName, char* retval) const
	{
		auto it = m_watchData.find(watchName);
		if (it == m_watchData.end())
		{
			const char* str = "n/a";
			std::strcpy(retval, str);
		}
		std::lock_guard<std::mutex> lock(m_watchMutex);
		std::strcpy(retval, it->second.c_str());
	}

};


} //namespace motordrive


