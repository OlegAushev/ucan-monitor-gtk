/**
 * @file cansocket.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "cansocket/cansocket.h"


namespace global {
extern std::shared_ptr<can::Socket> canSocket;
}


extern "C" {


///
///
/// 
void cansocket_connect(const char* interface, int bitrate)
{
	if (global::canSocket == nullptr)
	{
		return;
	}

	global::canSocket->connect(interface, bitrate);
}


///
///
/// 
void cansocket_disconnect()
{
	if (global::canSocket == nullptr)
	{
		return;
	}

	global::canSocket->disconnect();
}


}


