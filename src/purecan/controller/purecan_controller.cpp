#include "purecan_controller.h"


namespace purecan {

Controller::Controller(std::shared_ptr<can::Socket> socket)
	: _socket(socket)
{
	std::cout << "[purecan] Starting aux thread..." << std::endl;

	std::future<void> future_exit = _signal_exit_run_thread.get_future();
	_thread_run = std::thread(&Controller::_run, this, std::move(future_exit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


Controller::~Controller()
{
	std::cout << "[purecan] Sending signal to aux thread to stop..." << std::endl;

	_signal_exit_run_thread.set_value();
	_thread_run.join();	
}


void Controller::register_device(std::shared_ptr<IDevice> device)
{
	_devices.insert(device);

	for (const auto& [id, message] : device->_tx_message_list)
	{
		_recv_id_device_list.insert({id, device});
	}
}


void Controller::_run(std::future<void> future_exit)
{
	std::cout << "[purecan] Aux thread started." << std::endl;

	while (future_exit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* CONTROLLER TX */
		if (_is_tx_enabled)
		{
			for (auto& [id, message] : _tx_message_list)
			{
				if (message.period == std::chrono::milliseconds(0)) continue;
				if (now - message.timepoint < message.period) continue;

				can_frame frame;
				frame.can_id = id;
				auto data = message.creator();
				frame.len = data.size();
				memcpy(frame.data, data.data(), data.size());
				_socket->send(frame);

				message.timepoint = now;
			}
		}

		/* DEVICE's RX */
		for (auto& device : _devices)
		{
			device->_check_connection();
			device->_send();
		}

		/* RECV */
		can_frame frame;
		can::Error error = _socket->recv(frame);
		while (error == can::Error::none)
		{
			(void) std::async(&Controller::_on_frame_received, this, frame);
			error = _socket->recv(frame);
		}
	}

	std::cout << "[purecan] Aux thread stopped." << std::endl;
}


void Controller::_on_frame_received(const can_frame& frame)
{
	auto it = _recv_id_device_list.find(frame.can_id);
	if (it != _recv_id_device_list.end())
	{
		it->second->_handle_frame(frame);
	}
}

} // namespace purecan

