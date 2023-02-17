#pragma once


#include "ucanopen/server/ucanopen_server.h"
#include "srmdrive_def.h"
#include "../controller/srmdrive_controller.h"
#include "log/log.h"


namespace srmdrive {

extern const ucanopen::ObjectDictionary object_dictionary;
extern const ucanopen::ObjectDictionaryConfig object_dictionary_config;


class Server : public ucanopen::Server, public ucanopen::SdoSubscriber
{
protected:
	void _handle_tpdo1([[maybe_unused]] const ucanopen::can_payload& payload) {}
	void _handle_tpdo2([[maybe_unused]] const ucanopen::can_payload& payload) {}
	void _handle_tpdo3(const ucanopen::can_payload& payload);
	void _handle_tpdo4(const ucanopen::can_payload& payload);

	virtual ucanopen::FrameHandlingStatus handle_sdo(ucanopen::ODEntryIter entry,
					ucanopen::SdoType sdo_type,
					ucanopen::ExpeditedSdoData data) override final;
private:
	uint32_t _errors = 0;
	uint16_t _warnings = 0;

public:
	Controller controller;

	Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket);

	uint32_t errors() const { return _errors; }
	uint16_t warnings() const { return _warnings; }
};

} // namespace srmdrive

