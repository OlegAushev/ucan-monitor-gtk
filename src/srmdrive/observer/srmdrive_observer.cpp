/**
 * @file srmdrive_observer.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "srmdrive_observer.h"


namespace srmdrive {


///
///
///
Observer::Observer(ucanopen::IServer* driveServer)
	: m_driveServer(driveServer)
{
	for (auto entry : m_driveServer->watchEntriesList())
	{
		m_watchData.insert({entry, std::string{"..."}});
	}
}


///
///
///
void Observer::handleSdo(ucanopen::SdoType sdoType,
		ucanopen::ObjectDictionaryType::const_iterator entryIt,
		ucanopen::CobSdoData data)
{
	if (entryIt->second.category == m_driveServer->watchCategory)
	{
		auto dataType = entryIt->second.dataType;
		if (dataType != ucanopen::ODEntryDataType::OD_ENUM16)
		{
			m_watchData[entryIt->second.name] = data.toString(entryIt->second.dataType);
		}
		else if (entryIt->second.name == "DRIVE_STATE")
		{
			m_watchData[entryIt->second.name] = "not impl";
		}
	}
}


} // namespace srmdrive


