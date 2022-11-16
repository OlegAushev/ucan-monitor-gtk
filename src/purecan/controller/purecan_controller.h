/**
 * @file purecan_controller.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


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
	std::shared_ptr<can::Socket> m_socket;
	std::set<std::shared_ptr<IDevice>> m_devices;
	std::map<canid_t, std::shared_ptr<IDevice>> m_recvIdDeviceList;

	/* THREADS */
	std::thread m_threadRun;
	std::promise<void> m_signalExitRunThread;
	void run(std::future<void> futureExit);

	void onFrameReceived(const can_frame& frame);
public:
	Controller(std::shared_ptr<can::Socket> socket);
	~Controller();

	void registerDevice(std::shared_ptr<IDevice> device);
};








} // namespace purecan


