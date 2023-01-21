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
	bool _is_connection_ok = false;

	/* device --> controller */
	struct TxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds timeout;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		bool is_on_schedule;
		std::function<void(can_payload)> handler;
	};
	std::map<canid_t, TxMessageInfo> _tx_message_list;
	
	/* device <-- controller */
	bool _is_rx_enabled = true;
	struct RxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload_va(void)> creator; 
	};
	std::map<canid_t, RxMessageInfo> _rx_message_list;

public:
	IDevice(std::shared_ptr<can::Socket> socket);

	void register_tx_message(canid_t id, std::string_view name, std::chrono::milliseconds timeout, std::function<void(can_payload)> handler)
	{
		_tx_message_list.insert({id, {name, timeout, std::chrono::steady_clock::now() ,false, handler}});
	}

	void register_rx_message(canid_t id, std::string_view name, std::chrono::milliseconds period, std::function<can_payload_va(void)> creator)
	{
		_rx_message_list.insert({id, {name, period, std::chrono::steady_clock::now(), creator}});
	}

	void enable_rx_messages() { _is_rx_enabled = true; }
	void disable_rx_messages() { _is_rx_enabled = false; }
private:
	void _send();
	void _handle_frame(const can_frame& frame);
	void _check_connection();
};

} // namespace purecan

