/**
 * @file srmdrive_observer.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "ucanopen/client/ucanopen_client.h"


namespace srmdrive {



class Observer
{
private:
	ucanopen::IServer* m_driveServer;

	/* WATCH */
	std::map<std::string_view, std::string> m_watchData;
	mutable std::mutex m_watchMutex;
	std::vector<std::string_view> m_watchList;
	volatile bool m_isWatchEnabled{false};
	std::chrono::milliseconds m_watchPeriod{std::chrono::milliseconds(10)};
	
	void sendWatchRequest()
	{
		static size_t i = 0;
		if (m_isWatchEnabled)
		{
			m_driveServer->read("WATCH", "WATCH", m_watchList[i]);
			i = (i + 1) % m_watchList.size();		
		}
	}

	/* THREADS */
	std::thread m_threadRun;
	std::promise<void> m_signalExitRunThread;
	void run(std::future<void> futureExit);
	
public:
	Observer(ucanopen::IServer* driveServer);
	~Observer();
	
	void enableWatch()
	{
		m_isWatchEnabled = true;
#ifdef STD_COUT_ENABLED
		std::cout << "[srmdrive] Watch requests have been enabled." << std::endl;
#endif
	}

	void disableWatch()
	{
		m_isWatchEnabled = false;
#ifdef STD_COUT_ENABLED
		std::cout << "[srmdrive] Watch requests have been disabled." << std::endl;
#endif
	}

	void setWatchPeriod(std::chrono::milliseconds period)
	{
		m_watchPeriod = period;
#ifdef STD_COUT_ENABLED
		std::cout << "[srmdrive] Watch requests period = " << period << "." << std::endl;
#endif
	}

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
			return;
		}
		std::lock_guard<std::mutex> lock(m_watchMutex);
		std::strcpy(retval, it->second.c_str());
	}

	void handleSdo(ucanopen::SdoType sdoType,
			ucanopen::ObjectDictionaryType::const_iterator entryIt,
			ucanopen::CobSdoData data);

};


} //namespace srmdrive


