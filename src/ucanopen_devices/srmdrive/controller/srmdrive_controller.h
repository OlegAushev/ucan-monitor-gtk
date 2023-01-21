#pragma once


#include <algorithm>
#include "ucanopen/server/ucanopen_server.h"
#include "../server/srmdrive_def.h"


namespace srmdrive {

class Controller
{
private:
	ucanopen::Server* _drive_server;

	bool _is_run_enabled = false;
	bool _is_emergency_enabled = false;

	float _torque_pu_ref = 0;
	float _speed_ref = 0;

public:
	Controller(ucanopen::Server* drive_server);
	
	void powerup();
	void powerdown();

	void set_run(bool is_enabled);
	void set_emergency(bool is_enabled);

	void set_torque(double val_percents);
	void set_speed(double val);

	ucanopen::can_payload make_tpdo1();
	ucanopen::can_payload make_tpdo2();
};

} // namespace srmdrive

