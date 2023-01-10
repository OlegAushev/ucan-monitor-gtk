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
namespace Backend {

extern int main_enter();
extern void main_exit();
extern bool is_ready;

namespace Ucanopen {
extern string server_list[3];
extern string server;
extern string server_config_category;
}

}

// cansocket
extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();

// ucanopen
extern void ucanopen_client_set_node_id(uint nodeId);
extern void ucanopen_client_set_server_id(string serverName ,uint nodeId);
extern void ucanopen_client_set_tpdo_enabled(bool isEnabled);
extern void ucanopen_client_set_sync_enabled(bool isEnabled);
extern void ucanopen_client_set_sync_period(int period);
extern void ucanopen_client_set_watch_enabled(bool isEnabled);
extern void ucanopen_client_set_watch_period(int period);
extern void ucanopen_server_get_watch_value(string serverName, string watchName, char* buf, size_t len);
extern size_t ucanopen_server_get_config_categories(string serverName, char** buf, size_t countMax, size_t lenMax);
extern size_t ucanopen_server_get_config_entries(string serverName, string category, char** buf, size_t countMax, size_t lenMax);
extern bool ucanopen_server_is_heartbeat_ok(string serverName);
extern void ucanopen_server_get_nmt_state(string serverName, char* buf, size_t len);
extern bool ucanopen_server_is_tpdo_ok(string serverName, int tpdoNum);
extern void ucanopen_server_read(string serverName, string category, string subcategory, string name);
extern void ucanopen_server_write(string serverName, string category, string subcategory, string name, string value);
extern void ucanopen_server_exec(string serverName, string category, string subcategory, string name);

// logger
extern bool logger_get_message(string buf, size_t len);

// srmdrive
extern void srmdrive_controller_set_power_enabled(bool isEnabled);
extern void srmdrive_controller_set_run_enabled(bool isEnabled);
extern void srmdrive_controller_set_emergency_enabled(bool isEnabled);
extern void srmdrive_controller_set_torque(double valPu);
extern void srmdrive_controller_set_speed(double val);

/// bmsmain21
extern double bmsmain_tpdo1_get_voltage();
extern double bmsmain_tpdo1_get_current();
extern double bmsmain_tpdo1_get_charge();
extern double bmsmain_tpdo1_get_temp_max();
extern double bmsmain_tpdo1_get_temp_min();

/// ucanopen_devices
extern size_t ucanopen_devices_get_error_names(string serverName, char** buf, size_t countMax, size_t lenMax);
extern uint ucanopen_devices_get_error_code(string serverName);


int main(string[] args)
{
	var app = new CanMonitor.Application();
	return app.run(args);
}


