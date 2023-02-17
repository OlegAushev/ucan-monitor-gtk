#pragma once


#include "../impl/ucanopen_impl_server.h"
#include <vector>
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
	uint32_t get(std::future<void> signal_terminate) const;
	virtual int handle_sdo(SdoType sdo_type, ObjectDictionaryEntries::const_iterator entry_iter, ExpeditedSdoData sdo_data);
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
	std::string get(std::future<void> signal_terminate) const;
	virtual int handle_sdo(SdoType sdo_type, ObjectDictionaryEntries::const_iterator entry_iter, ExpeditedSdoData sdo_data);
};

}

} // namespace ucanopen 

