/**
 * @file purecan_device.h
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
#include "../purecan_def.h"
#include <map>
#include <functional>


namespace purecan {


class Device
{
private:
	std::shared_ptr<can::Socket> m_socket;
	bool m_isConnectionOk{false};

	/* device --> controller */
	struct TxMessageInfo
	{
		std::string_view name;
		canid_t id;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool isOnSchedule;
		std::function<void(can_payload)> handler;
	};
	std::map<canid_t, TxMessageInfo> m_txMessageList;
	
	/* device <-- controller */
	bool m_isRxEnabled{true};
	struct RxMessageInfo
	{
		std::string_view name;
		canid_t id;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<std::vector<uint8_t>(void)> creator; 
	};
	std::map<canid_t, RxMessageInfo> m_rxMessageList;

public:
	Device(std::shared_ptr<can::Socket> socket);

private:
	void send();
	void handleFrame(const can_frame& frame);
	void checkConnection();
};


} // namespace purecan


