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
	: m_socket(socket)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[purecan] Starting aux thread..." << std::endl;
#endif
	std::future<void> futureExit = m_signalExitRunThread.get_future();
	m_threadRun = std::thread(&Controller::run, this, std::move(futureExit));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


///
///
///
Controller::~Controller()
{
#ifdef STD_COUT_ENABLED
	std::cout << "[purecan] Sending signal to aux thread to stop..." << std::endl;
#endif
	m_signalExitRunThread.set_value();
	m_threadRun.join();	
}


///
///
///
void Controller::registerDevice(std::shared_ptr<IDevice> device)
{
	m_devices.insert(device);

	for (const auto& [id, message] : device->m_txMessageList)
	{
		m_recvIdDeviceList.insert({id, device});
	}
}


///
///
///
void Controller::run(std::future<void> futureExit)
{
#ifdef STD_COUT_ENABLED
	std::cout << "[purecan] Aux thread has started." << std::endl;
#endif

	while (futureExit.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		for (auto& device : m_devices)
		{
			device->checkConnection();
			device->send();
		}

		can_frame frame;
		can::Error recvErr = m_socket->recv(frame);
		while (recvErr == can::Error::NO_ERROR)
		{
			(void) std::async(&Controller::onFrameReceived, this, frame);
			recvErr = m_socket->recv(frame);
		}
	}

#ifdef STD_COUT_ENABLED
	std::cout << "[purecan] Aux thread has stopped." << std::endl;
#endif
}


///
///
///
void Controller::onFrameReceived(const can_frame& frame)
{
	auto it = m_recvIdDeviceList.find(frame.can_id);
	if (it != m_recvIdDeviceList.end())
	{
		it->second->handleFrame(frame);
	}
}


} // namespace purecan


