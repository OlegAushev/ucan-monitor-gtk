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


namespace plaincan {


/// 
///
///
Device::Device(std::shared_ptr<can::Socket> socket)
	: m_socket(socket)
{

}


///
///
///
void Device::send()
{
	auto now = std::chrono::steady_clock::now();

	if (!m_isRxEnabled) return;

	for (auto& msg : m_rxMessageList)
	{
		if (msg.second.period == std::chrono::milliseconds(0)) continue;
		if (now - msg.second.timepoint < msg.second.period) continue;

		can_frame frame;
		frame.can_id = msg.second.id;
		auto data = msg.second.creator();
		memcpy(frame.data, data.data(), data.size());
		m_socket->send(frame);

		msg.second.timepoint = now;
	}
}


///
///
///
void Device::handleFrame(can_frame frame)
{
	
}






}


