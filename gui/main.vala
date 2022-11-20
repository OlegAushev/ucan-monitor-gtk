/* main.vala
 *
 * Copyright 2022 Oleg Aushev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


extern string GIT_DESCRIBE;
extern int main_enter();
extern void main_exit();
extern bool g_isBackendReady;

extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();

//  extern void ucanopen_client_set_tpdo_enabled(bool isEnabled);

//  extern bool srmdrive_is_connection_ok();
//  extern bool srmdrive_is_tpdo_ok(int tpdoNum);

//  extern void srmdrive_controller_set_power_enabled(bool isEnabled);
//  extern void srmdrive_controller_set_run_enabled(bool isEnabled);
//  extern void srmdrive_controller_set_emergency_enabled(bool isEnabled);
//  extern void srmdrive_controller_set_torque(double valPu);
//  extern void srmdrive_controller_set_speed(double val);

//  extern void srmdrive_observer_get_watch_value(string name, string value);
//  extern void srmdrive_observer_set_watch_enabled(bool isEnabled);
//  extern void srmdrive_observer_set_watch_period(int period);

extern void atv_vcm_set_torque(double val);
extern void atv_vcm_set_hv_power_supply(bool state);
extern void atv_vcm_set_relay_plus_output(bool state);

extern double atv_leaf_inverter_get_torque();


int main(string[] args)
{
	var app = new CanMonitor.Application();
	return app.run(args);
}


