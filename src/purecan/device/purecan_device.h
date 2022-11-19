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


class IDevice
{
	friend class Controller;
private:
	std::shared_ptr<can::Socket> m_socket;
	bool m_isConnectionOk{false};

	/* device --> controller */
	struct TxMessageInfo
	{
		std::string_view name;
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
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload_va(void)> creator; 
	};
	std::map<canid_t, RxMessageInfo> m_rxMessageList;

public:
	IDevice(std::shared_ptr<can::Socket> socket);

	void registerTxMessage(canid_t id, std::string_view name, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler)
	{
		m_txMessageList.insert({id, {name, timeout, std::chrono::steady_clock::now() ,false, handler}});
	}

	void registerRxMessage(canid_t id, std::string_view name, std::chrono::milliseconds period, std::function<can_payload_va(void)> creator)
	{
		m_rxMessageList.insert({id, {name, period, std::chrono::steady_clock::now(), creator}});
	}

	void enableRxMessages() { m_isRxEnabled = true; }
	void disableRxMessages() { m_isRxEnabled = false; }

private:
	void send();
	void handleFrame(const can_frame& frame);
	void checkConnection();
};


} // namespace purecan


