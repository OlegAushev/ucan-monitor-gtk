/**
 * @file c_ucanopen.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucanopen/ucanopen_client.h"


extern std::shared_ptr<ucanopen::Client> g_ucanClient;


extern "C" {


///
///
///
void ucanopen_client_set_tpdo_state(bool state)
{
	if (state)
	{
		g_ucanClient->enableTpdo();
	}
	else
	{
		g_ucanClient->disableTpdo();
	}
}


}


