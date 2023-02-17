#include "ucanopen_server_od_utils.h"


namespace ucanopen {

namespace utils {


uint32_t SerialNumberGetter::get(std::future<void> signal_terminate) const
	{
		_server->read("sys", "info", "serial_number");
		while (signal_terminate.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout
				&& _serial_number == 0)
		{/*WAIT*/}
		return _serial_number;
	}

FrameHandlingStatus SerialNumberGetter::handle_sdo(SdoType sdo_type, ODEntryIter entry_iter, ExpeditedSdoData sdo_data)
{
	if (sdo_type == SdoType::response_to_read
		&& entry_iter->second.category == "sys"
		&& entry_iter->second.subcategory == "info"
		&& entry_iter->second.name == "serial_number")
	{
		_serial_number = sdo_data.u32();
		return FrameHandlingStatus::success;
	}
	return FrameHandlingStatus::irrelevant_frame;
}


std::string DeviceNameGetter::get(std::future<void> signal_terminate) const
{
	_server->read("sys", "info", "device_name");
	while (signal_terminate.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout
			&& !_ready)
	{/*WAIT*/}
	return _device_name;
}

FrameHandlingStatus DeviceNameGetter::handle_sdo(SdoType sdo_type, ODEntryIter entry_iter, ExpeditedSdoData sdo_data)
{
	if (sdo_type == SdoType::response_to_read
		&& entry_iter->second.category == "sys"
		&& entry_iter->second.subcategory == "info"
		&& entry_iter->second.name == "device_name")
	{
		std::array<char, 4> char_arr;
		memcpy(char_arr.data(), &sdo_data, 4);
		for (auto ch : char_arr)
		{
			_char_vec.push_back(ch);
			if (ch == '\0')
			{
				_device_name = std::string(_char_vec.data());
				_ready = true;
				break;
			}
		}

		if (!_ready)
		{
			_server->read("sys", "info", "device_name");
		}

		return FrameHandlingStatus::success;
	}
	return FrameHandlingStatus::irrelevant_frame;
}

} // namespace utils

} // namespace ucanopen

