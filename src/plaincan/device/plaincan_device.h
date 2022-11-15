/**
 * @file plaincan_device.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "cansocket/cansocket.h"
#include <map>


namespace plaincan {


class Device
{
private:
	std::shared_ptr<can::Socket> m_socket;

	struct TxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool isOnSchedule;
	};
	std::map<canid_t, TxMessageInfo> m_txMessageList;
	
	
	
	
	std::map<canid_t, std::string_view> m_recvIdNameList;
public:
	Device(std::shared_ptr<can::Socket> socket,
			const std::map<canid_t, std::string_view>& sendIdNameList,
			const std::map<canid_t, std::string_view>& recvIdNameList);




};


}


