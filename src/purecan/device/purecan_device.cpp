#include "purecan_device.h"


namespace purecan {

IDevice::IDevice(std::shared_ptr<can::Socket> socket)
	: _socket(socket)
{

}


void IDevice::_send()
{
	auto now = std::chrono::steady_clock::now();

	if (!_is_rx_enabled) return;

	for (auto& [id, message] : _rx_message_list)
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


void IDevice::_handle_frame(const can_frame& frame)
{
	for (auto& [id, message] : _tx_message_list)
	{
		if (frame.can_id != id) continue;

		message.timepoint = std::chrono::steady_clock::now();
		message.is_on_schedule = true;
		can_payload data{};
		std::copy(frame.data, std::next(frame.data, frame.can_dlc), data.begin());
		message.handler(data);
	}
}


void IDevice::_check_connection()
{
	bool is_connection_ok = true;
	auto now = std::chrono::steady_clock::now();

	for (auto& [id, message] : _tx_message_list)
	{
		if (message.timeout == std::chrono::milliseconds(0)) continue;
		if ((now - message.timepoint) > message.timeout)
		{
			message.is_on_schedule = false;
			is_connection_ok = false;
		}
	}

	_is_connection_ok = is_connection_ok;
}

} // namespace purecan

