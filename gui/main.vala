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

// core backend
namespace backend {
extern int main_enter();
extern void main_exit();
extern bool isReady;
extern string ucanopenServer;
}

// cansocket
extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();

// ucanopen
extern void ucanopen_client_set_nodeid(uint nodeId);
extern void ucanopen_client_set_serverid(string name ,uint nodeId);
extern void ucanopen_client_set_tpdo_enabled(bool isEnabled);
extern void ucanopen_client_set_watch_enabled(bool isEnabled);
extern void ucanopen_client_set_watch_period(int period);
extern void ucanopen_server_get_watch_value(string name, string watch, char* buf, size_t len);
extern size_t ucanopen_server_get_conf_categories(string name, char** buf, size_t size, size_t len);
extern size_t ucanopen_server_get_conf_entries(string name, string category, char** buf, size_t size, size_t len);

// logger
extern bool logger_get_message(string buf, size_t len);

// srmdrive
extern bool srmdrive_is_heartbeat_ok();
extern void srmdrive_get_nmt_state(string state);
extern bool srmdrive_is_tpdo_ok(int tpdoNum);
extern void srmdrive_controller_set_power_enabled(bool isEnabled);
extern void srmdrive_controller_set_run_enabled(bool isEnabled);
extern void srmdrive_controller_set_emergency_enabled(bool isEnabled);
extern void srmdrive_controller_set_torque(double valPu);
extern void srmdrive_controller_set_speed(double val);
extern void srmdrive_observer_get_watch_value(string name, string buf, size_t len);




int main(string[] args)
{
	var app = new CanMonitor.Application();
	return app.run(args);
}


