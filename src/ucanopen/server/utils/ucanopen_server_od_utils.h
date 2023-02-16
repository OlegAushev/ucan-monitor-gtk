#pragma once


#include "../impl/ucanopen_impl_server.h"
#include <future>


namespace ucanopen {

namespace utils {

class SerialNumberGetter : public impl::SdoSubscriber
{
private:
	impl::Server* const _server;
	uint32_t _serial_number = 0;
public:
	SerialNumberGetter(impl::Server* server, impl::SdoPublisher* publisher)
		: impl::SdoSubscriber(publisher)
		, _server(server)
	{}
	
	uint32_t get(std::future<void> signal_terminate) const
	{
		_server->read("sys", "info", "serial_number");
		while (signal_terminate.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout
				&& _serial_number == 0)
		{/*WAIT*/}
		return _serial_number;
	}

	virtual int handle_sdo(SdoType sdo_type, ObjectDictionaryEntries::const_iterator entry_iter, ExpeditedSdoData sdo_data)
	{
		if (sdo_type == SdoType::response_to_read
			&& entry_iter->second.category == "sys"
			&& entry_iter->second.subcategory == "info"
			&& entry_iter->second.name == "serial_number")
		{
			_serial_number = sdo_data.u32();
			return 0;
		}
		return 1;
	}
};


class DeviceNameGetter : public impl::SdoSubscriber
{
private:
	impl::Server* const _server;
	std::vector<char> _char_vec;
	std::string _device_name = "n/a";
	bool _ready = false;
public:
	DeviceNameGetter(impl::Server* server, impl::SdoPublisher* publisher)
		: impl::SdoSubscriber(publisher)
		, _server(server)
	{}
	
	std::string get(std::future<void> signal_terminate) const
	{
		_server->read("sys", "info", "device_name");
		while (signal_terminate.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout
				&& !_ready)
		{/*WAIT*/}
		return _device_name;
	}

	virtual int handle_sdo(SdoType sdo_type, ObjectDictionaryEntries::const_iterator entry_iter, ExpeditedSdoData sdo_data)
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

			return 0;
		}
		return 1;
	}
};

}

} // namespace ucanopen 

