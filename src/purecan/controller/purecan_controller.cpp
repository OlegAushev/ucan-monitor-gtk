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

	for (const auto& [id, msg] : device->m_txMessageList)
	{
		m_recvIdDeviceList.insert({id, device});
	}
}


///
///
///
void Controller::run(std::future<void> futureExit)
{

}


} // namespace purecan


