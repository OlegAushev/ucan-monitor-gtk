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
#include <functional>


namespace plaincan {


class Device
{
private:
	std::shared_ptr<can::Socket> m_socket;

	/* device --> controller */
	struct TxMessageInfo
	{
		std::string_view name;
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool isOnSchedule;
		std::function<void(can_frame)> handler;
	};
	std::map<canid_t, TxMessageInfo> m_txMessageList;
	
	/* device <-- controller */
	struct RxMessageInfo
	{
		std::string_view name;
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_frame(void)> creator; 
	};
	std::map<canid_t, RxMessageInfo> m_rpdoList;

public:
	Device(std::shared_ptr<can::Socket> socket);

private:
	void send();
	void processFrame(can_frame frame);
	void checkConnection();
};


}


