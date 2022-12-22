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
	: _socket(socket)
{

}


///
///
///
void IDevice::send()
{
	auto now = std::chrono::steady_clock::now();

	if (!_isRxEnabled) return;

	for (auto& [id, message] : _rxMessageList)
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


///
///
///
void IDevice::handleFrame(const can_frame& frame)
{
	for (auto& [id, message] : _txMessageList)
	{
		if (frame.can_id != id) continue;

		message.timepoint = std::chrono::steady_clock::now();
		message.isOnSchedule = true;
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
		message.handler(data);
	}
}


///
///
///
void IDevice::checkConnection()
{
	bool isConnectionOk = true;
	auto now = std::chrono::steady_clock::now();

	for (auto& [id, message] : _txMessageList)
	{
		if (message.timeout == std::chrono::milliseconds(0)) continue;
		if ((now - message.timepoint) > message.timeout)
		{
			message.isOnSchedule = false;
			isConnectionOk = false;
		}
	}

	_isConnectionOk = isConnectionOk;
}


} // namespace purecan


