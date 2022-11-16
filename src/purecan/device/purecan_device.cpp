/**
 * @file purecan_device.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "purecan_device.h"


namespace purecan {


/// 
///
///
IDevice::IDevice(std::shared_ptr<can::Socket> socket)
	: m_socket(socket)
{

}


///
///
///
void IDevice::send()
{
	auto now = std::chrono::steady_clock::now();

	if (!m_isRxEnabled) return;

	for (auto& msg : m_rxMessageList)
	{
		if (msg.second.period == std::chrono::milliseconds(0)) continue;
		if (now - msg.second.timepoint < msg.second.period) continue;

		can_frame frame;
		frame.can_id = msg.first;
		auto data = msg.second.creator();
		memcpy(frame.data, data.data(), data.size());
		m_socket->send(frame);

		msg.second.timepoint = now;
	}
}


///
///
///
void IDevice::handleFrame(const can_frame& frame)
{
	for (auto& msg : m_txMessageList)
	{
		if (frame.can_id != msg.first) continue;

		msg.second.timepoint = std::chrono::steady_clock::now();
		msg.second.isOnSchedule = true;
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
		msg.second.handler(data);
	}
}


///
///
///
void IDevice::checkConnection()
{
	bool isConnectionOk = true;
	auto now = std::chrono::steady_clock::now();

	for (auto& msg : m_txMessageList)
	{
		if (msg.second.timeout == std::chrono::milliseconds(0)) continue;
		if ((now - msg.second.timepoint) > msg.second.timeout)
		{
			msg.second.isOnSchedule = false;
			isConnectionOk = false;
		}
	}

	m_isConnectionOk = isConnectionOk;
}




} // namespace purecan


