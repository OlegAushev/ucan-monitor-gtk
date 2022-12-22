/**
 * @file purecan_controller.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "purecan_controller.h"


namespace purecan {


///
///
///
Controller::Controller(std::shared_ptr<can::Socket> socket)
	: _socket(socket)
{
	std::cout << "[purecan] Starting aux thread..." << std::endl;

	std::future<void> futureExit = _signalExitRunThread.get_future();
	_threadRun = std::thread(&Controller::run, this, std::move(futureExit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


///
///
///
Controller::~Controller()
{
	std::cout << "[purecan] Sending signal to aux thread to stop..." << std::endl;

	_signalExitRunThread.set_value();
	_threadRun.join();	
}


///
///
///
void Controller::registerDevice(std::shared_ptr<IDevice> device)
{
	_devices.insert(device);

	for (const auto& [id, message] : device->_txMessageList)
	{
		_recvIdDeviceList.insert({id, device});
	}
}


///
///
///
void Controller::run(std::future<void> futureExit)
{
	std::cout << "[purecan] Aux thread started." << std::endl;

	while (futureExit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		auto now = std::chrono::steady_clock::now();

		/* CONTROLLER TX */
		if (_isTxEnabled)
		{
			for (auto& [id, message] : _txMessageList)
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
			device->checkConnection();
			device->send();
		}

		/* RECV */
		can_frame frame;
		can::Error recvErr = _socket->recv(frame);
		while (recvErr == can::Error::NoError)
		{
			(void) std::async(&Controller::onFrameReceived, this, frame);
			recvErr = _socket->recv(frame);
		}
	}

	std::cout << "[purecan] Aux thread stopped." << std::endl;
}


///
///
///
void Controller::onFrameReceived(const can_frame& frame)
{
	auto it = _recvIdDeviceList.find(frame.can_id);
	if (it != _recvIdDeviceList.end())
	{
		it->second->handleFrame(frame);
	}
}


} // namespace purecan


