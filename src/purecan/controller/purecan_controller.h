#pragma once


#include <thread>
#include <future>

#include "cansocket/cansocket.h"
#include "../purecan_def.h"
#include "purecan/device/purecan_device.h"


namespace purecan {

class Controller
{
private:
	std::shared_ptr<can::Socket> _socket;
	std::set<std::shared_ptr<IDevice>> _devices;
	std::map<canid_t, std::shared_ptr<IDevice>> _recv_id_device_list;

	/* controller -> device */
	bool _is_tx_enabled = true;
	struct TxMessageInfo
	{
		std::string_view name;
		std::chrono::milliseconds period;
		std::chrono::time_point<std::chrono::steady_clock> timepoint;
		std::function<can_payload_va(void)> creator; 
	};
	std::map<canid_t, TxMessageInfo> _tx_message_list;

	/* THREADS */
	std::thread _thread_run;
	std::promise<void> _signal_exit_run_thread;
	void _run(std::future<void> future_exit);

	void _on_frame_received(const can_frame& frame);
public:
	Controller(std::shared_ptr<can::Socket> socket);
	~Controller();
	void register_device(std::shared_ptr<IDevice> device);

	void register_tx_message(canid_t id, std::string_view name, std::chrono::milliseconds period, std::function<can_payload_va(void)> creator)
	{
		_tx_message_list.insert({id, {name, period, std::chrono::steady_clock::now(), creator}});
	}

	void enable_tx_messages() { _is_tx_enabled = true; }
	void disable_tx_messages() { _is_tx_enabled = false; }
};

} // namespace purecan

