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
			&& entry_iter->second.name == "serial_number")
		{
			_serial_number = sdo_data.u32();
			return 0;
		}
		return 1;
	}
};

}

} // namespace ucanopen 

