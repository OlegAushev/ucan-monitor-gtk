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
	std::set<std::string_view> watchEntries =
			m_ucanClient->serverNodes.at(ucanopen::ServerNode::Name::C2000).watchEntriesList();

	for (auto entry : watchEntries)
	{
		m_watchData.insert({entry, std::string{"..."}});
	}
}


} // namespace motordrive


