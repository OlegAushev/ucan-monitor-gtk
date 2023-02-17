#pragma once


#include "ucanopen/server/ucanopen_server.h"
#include "bmsmain_def.h"


namespace bmsmain {

class Server : public ucanopen::Server
{
private:
	double _current = 0;
	double _voltage = 0;
	unsigned int _charge = 0;
	double _temp_min = 0;
	double _temp_max = 0;

protected:
	void _handle_tpdo1(const ucanopen::can_payload& payload);
public:
	Server(const std::string& name, ucanopen::NodeId node_id, std::shared_ptr<can::Socket> socket);

	double current() const { return _current; }
	double voltage() const { return _voltage; }
	unsigned int charge() const { return _charge; }
	double temp_min() const { return _temp_min; }
	double temp_max() const { return _temp_max; }
};

}

