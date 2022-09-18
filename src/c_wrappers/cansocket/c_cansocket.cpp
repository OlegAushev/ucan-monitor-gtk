/**
 * @file c_cansocket.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "c_cansocket.h"
#include "cansocket/cansocket.h"


extern std::shared_ptr<can::Socket> g_canSocket;


///
///
/// 
void cansocket_connect()
{
	if (g_canSocket == nullptr)
	{
		return;
	}

	g_canSocket->connect("can0", 125000);
}


///
///
/// 
void cansocket_disconnect()
{
	if (g_canSocket == nullptr)
	{
		return;
	}

	g_canSocket->disconnect();
}


