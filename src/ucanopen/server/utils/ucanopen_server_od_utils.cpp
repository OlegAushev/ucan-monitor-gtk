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


StringReader::StringReader(impl::Server* server, impl::SdoPublisher* publisher,
	std::string_view category, std::string_view subcategory, std::string_view name)
	: impl::SdoSubscriber(publisher)
	, _server(server)
	, _category(category), _subcategory(subcategory), _name(name)
{
	ODEntryIter entry_iter;
	if (_server->find_od_entry(_category, _subcategory, _name, entry_iter, traits::check_read_perm{}) != ODAccessStatus::success)
	{
		_ready = true;
		return;
	}
	if (entry_iter->second.type != OD_STRING)
	{
		_ready = true;
		return;
	}

	if (_server->read(_category, _subcategory, _name) != ODAccessStatus::success)
	{
		_ready = true;
	}
}


std::string StringReader::get(std::future<void> signal_terminate) const
{
	while (signal_terminate.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout
			&& !_ready)
	{/*WAIT*/}
	return _result;
}

FrameHandlingStatus StringReader::handle_sdo(SdoType sdo_type, ODEntryIter entry_iter, ExpeditedSdoData sdo_data)
{
	if (sdo_type == SdoType::response_to_read
		&& entry_iter->second.category == _category
		&& entry_iter->second.subcategory == _subcategory
		&& entry_iter->second.name == _name)
	{
		std::array<char, 4> char_arr;
		memcpy(char_arr.data(), &sdo_data, 4);
		for (auto ch : char_arr)
		{
			_charbuf.push_back(ch);
			if (ch == '\0')
			{
				_result = std::string(_charbuf.data());
				_ready = true;
				break;
			}
		}

		if (!_ready)
		{
			_server->read(_category, _subcategory, _name);
		}

		return FrameHandlingStatus::success;
	}
	return FrameHandlingStatus::irrelevant_frame;	
}

} // namespace utils

} // namespace ucanopen

