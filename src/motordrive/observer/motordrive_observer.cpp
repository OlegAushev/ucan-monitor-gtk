/**
 * @file motordrive_observer.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "motordrive_observer.h"


namespace motordrive {


///
///
///
Observer::Observer(std::shared_ptr<ucanopen::Client> ucanClient)
	: m_ucanClient(ucanClient)
{
	m_watchList = m_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).watchEntriesList();
	for (auto entry : m_watchList)
	{
		m_watchData.insert({entry, std::string{"..."}});
	}

	std::future<void> futureExit = m_signalExitRunThread.get_future();
	m_threadRun = std::thread(&Observer::run, this, std::move(futureExit));
}


///
///
///
Observer::~Observer()
{
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Sending signal to observer aux thread to stop..." << std::endl;
#endif
	m_signalExitRunThread.set_value();
	m_threadRun.join();	
}


///
///
///
void Observer::run(std::future<void> futureExit)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Observer aux thread has started." << std::endl;
#endif

	while (futureExit.wait_for(m_watchPeriod) == std::future_status::timeout)
	{
		sendWatchRequest();
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] Observer aux thread has stopped." << std::endl;
#endif
}


///
///
///
void Observer::processSdo(ucanopen::SdoType sdoType,
		ucanopen::ObjectDictionaryType::const_iterator entryIt,
		ucanopen::CobSdoData data)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[motordrive] SDO received." << std::endl;
#endif
}




} // namespace motordrive


