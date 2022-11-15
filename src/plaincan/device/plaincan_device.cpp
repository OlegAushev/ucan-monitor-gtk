/**
 * @file plaincan_device.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "plaincan_device.h"


namespace plaincan {


Device::Device(std::shared_ptr<can::Socket> socket,
		const std::map<canid_t, std::string_view>& sendIdNameList,
		const std::map<canid_t, std::string_view>& recvIdNameList)
	: m_socket(socket)
	//, m_sendIdNameList(sendIdNameList)
	, m_recvIdNameList(recvIdNameList)
{

}


}


