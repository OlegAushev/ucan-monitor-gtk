#pragma once


#include <ucanopen/server/ucanopen_server.h>
#include "crd600_def.h"
#include <logger/logger.h>


namespace crd600 {


extern const ucanopen::ObjectDictionary object_dictionary;
extern const ucanopen::ObjectDictionaryConfig object_dictionary_config;


class Server : public ucanopen::Server
{
private:
	uint32_t _errors = 0;
	uint16_t _warnings = 0;

public:
	Server(const std::string& name,	ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket);
	
	uint32_t errors() const { return _errors; }
	uint16_t warnings() const { return _warnings; }
protected:
	virtual void _handle_tsdo(ucanopen::SdoType sdo_type,
			ucanopen::ObjectDictionary::const_iterator entry_iter,
			ucanopen::CobSdoData sdo_data) override final;
private:
	void _handle_tpdo1(const ucanopen::can_payload& payload);
	void _handle_tpdo2([[maybe_unused]] const ucanopen::can_payload& payload) {}
	void _handle_tpdo3([[maybe_unused]] const ucanopen::can_payload& payload) {}
	void _handle_tpdo4(const ucanopen::can_payload& payload);

	ucanopen::can_payload _create_rpdo1()
	{
		//static unsigned int counter = 0;
		//CobRpdo1 message{.counter = counter, ._reserved = 0, .value = _server_values[0]};
		//counter = (counter + 1) % 4;
		//return ucanopen::to_payload<CobRpdo1>(message);
	}

	ucanopen::can_payload _create_rpdo2()
	{
		//static unsigned int counter = 0;
		//CobRpdo2 message{.counter = counter, ._reserved = 0, .value = _server_values[1]};
		//counter = (counter + 1) % 4;
		//return ucanopen::to_payload<CobRpdo2>(message);
	}

public:
	struct Tpdo1
	{
		bool status_drive1_run;
		bool status_drive2_run;
		bool status_error;
		bool status_warning;
		bool status_overheat;
		std::string drive1_ref;
		std::string drive2_ref;
		std::string control_loop_type;
		std::string drive1_state;
		std::string drive2_state;
	} tpdo1;
};

} // namespace crd600

