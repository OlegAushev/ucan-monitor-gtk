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
	std::shared_ptr<can::Socket> _socket;
	bool _isConnectionOk{false};

	/* device --> controller */
	struct TxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool isOnSchedule;
		std::function<void(can_payload)> handler;
	};
	std::map<canid_t, TxMessageInfo> _txMessageList;
	
	/* device <-- controller */
	bool _isRxEnabled{true};
	struct RxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload_va(void)> creator; 
	};
	std::map<canid_t, RxMessageInfo> _rxMessageList;

public:
	/**
	 * @brief Construct a new IDevice object
	 * 
	 * @param socket 
	 */
	IDevice(std::shared_ptr<can::Socket> socket);

	/**
	 * @brief 
	 * 
	 * @param id 
	 * @param name 
	 * @param timeout 
	 * @param handler 
	 */
	void registerTxMessage(canid_t id, std::string_view name, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler)
	{
		_txMessageList.insert({id, {name, timeout, std::chrono::steady_clock::now() ,false, handler}});
	}

	/**
	 * @brief 
	 * 
	 * @param id 
	 * @param name 
	 * @param period 
	 * @param creator 
	 */
	void registerRxMessage(canid_t id, std::string_view name, std::chrono::milliseconds period, std::function<can_payload_va(void)> creator)
	{
		_rxMessageList.insert({id, {name, period, std::chrono::steady_clock::now(), creator}});
	}

	/**
	 * @brief 
	 * 
	 */
	void enableRxMessages() { _isRxEnabled = true; }

	/**
	 * @brief 
	 * 
	 */
	void disableRxMessages() { _isRxEnabled = false; }

private:
	void send();
	void handleFrame(const can_frame& frame);
	void checkConnection();
};


} // namespace purecan


