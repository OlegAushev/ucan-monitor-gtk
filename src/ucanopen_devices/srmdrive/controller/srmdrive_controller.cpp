#include "srmdrive_controller.h"


namespace srmdrive {

Controller::Controller(ucanopen::Server* driveServer)
	: _drive_server(driveServer)
{}


void Controller::powerup()
{
	_drive_server->exec("DRIVE", "CONTROL", "POWERUP");
	std::cout << "[srmdrive] Power UP motor drive." << std::endl;
}


void Controller::powerdown()
{
	_drive_server->exec("DRIVE", "CONTROL", "POWERDOWN");
	std::cout << "[srmdrive] Power DOWN motor drive." << std::endl;
}


void Controller::set_run(bool is_enabled)
{
	_is_run_enabled = is_enabled;
	std::cout << "[srmdrive] RUN state: " << is_enabled << std::endl;
}


void Controller::set_emergency(bool is_enabled)
{
	_is_emergency_enabled = is_enabled;
	std::cout << "[srmdrive] EMERGENCY state: " << is_enabled << std::endl;
}


void Controller::set_torque(double val_pu)
{
	_torque_pu_ref = std::clamp(val_pu, -1.0, 1.0);
	std::cout << "[srmdrive] Torque reference: " << _torque_pu_ref * 100.0 << "%" << std::endl;
}


void Controller::set_speed(double val)
{
	_speed_ref = val;
	std::cout << "[srmdrive] Speed reference: " << _speed_ref << "rpm" << std::endl;
	_drive_server->write("WATCH", "WATCH", "SPEED_RPM", ucanopen::CobSdoData(_speed_ref));
}


ucanopen::can_payload Controller::make_tpdo1()
{
	CobRpdo1 message{};

	message.run = ((_is_run_enabled) ? 1 : 0);
	message.emergency_stop = ((_is_emergency_enabled) ? 1 : 0);

	return ucanopen::to_payload(message);
}


ucanopen::can_payload Controller::make_tpdo2()
{
	CobRpdo2 message{};

	message.torque = ((_torque_pu_ref > 0) ? _torque_pu_ref * 32767 : _torque_pu_ref * 32768);

	return ucanopen::to_payload(message);
}

} // namespace srmdrive

