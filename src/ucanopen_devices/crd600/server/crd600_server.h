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

	ucanopen::can_payload _create_rpdo1();
	ucanopen::can_payload _create_rpdo2();
	ucanopen::can_payload _create_rpdo3();
public:
	bool drive1_run = false;
	bool drive2_run = false;
	bool emergency_stop = false;
	int16_t drive1_speed_ref = 0;
	int16_t drive1_torque_ref = 0;
	int16_t drive2_speed_ref = 0;
	int16_t drive2_torque_ref = 0;

	void set_drive1_speed_ref(double val)
	{
		if (fabs(val) > 8000.0) return;
		drive1_speed_ref = val;
	}

	void set_drive2_speed_ref(double val)
	{
		if (fabs(val) > 8000.0) return;
		drive2_speed_ref = val;
	}

	void set_drive1_torque_ref(double val_pu)
	{
		if (fabs(val_pu) > 1.0) return;
		drive1_torque_ref = 32767.0 * val_pu;
	}

	void set_drive2_torque_ref(double val_pu)
	{
		if (fabs(val_pu) > 1.0) return;
		drive2_torque_ref = 32767.0 * val_pu;
	}

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

