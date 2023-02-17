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
extern string server_list[4];
extern string server;
extern string server_config_category;
}

}

// cansocket
extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();

// ucanopen
extern void ucanopen_client_set_node_id(uint node_id);
extern void ucanopen_client_set_server_id(string server_name ,uint node_id);
extern void ucanopen_client_set_tpdo_enabled(bool is_enabled);
extern void ucanopen_client_set_server_rpdo_enabled(bool is_enabled);
extern void ucanopen_client_set_sync_enabled(bool is_enabled);
extern void ucanopen_client_set_sync_period(int period);
extern void ucanopen_client_set_watch_enabled(bool is_enabled);
extern void ucanopen_client_set_watch_period(int period);
extern void ucanopen_server_get_watch_value(string server_name, string watch_name, char* retbuf, size_t bufsize);
extern size_t ucanopen_server_get_config_categories(string server_name, char** retbuf, size_t str_count, size_t str_size);
extern size_t ucanopen_server_get_config_objects(string server_name, string category, char** retbuf, size_t str_count, size_t str_size);
extern bool ucanopen_server_is_heartbeat_ok(string server_name);
extern void ucanopen_server_get_nmt_state(string server_name, char* retbuf, size_t bufsize);
extern bool ucanopen_server_is_tpdo_ok(string server_name, uint tpdo_num);
extern ulong ucanopen_server_get_tpdo_data(string server_name, uint tpdo_num);
extern void ucanopen_server_read(string server_name, string category, string subcategory, string name);
extern void ucanopen_server_write(string server_name, string category, string subcategory, string name, string value);
extern void ucanopen_server_exec(string server_name, string category, string subcategory, string name);

extern void ucanopen_server_read_string(string server_name, string category, string subcategory, string name, uint timeout_ms, char* retbuf, size_t bufsize);
extern void ucanopen_server_read_numval(string server_name, string category, string subcategory, string name, uint timeout_ms, char* retbuf, size_t bufsize);

// logger
extern bool log_get_message(string retbuf, size_t bufsize);

// srmdrive
extern void srmdrive_controller_set_power_enabled(bool is_enabled);
extern void srmdrive_controller_set_run_enabled(bool is_enabled);
extern void srmdrive_controller_set_emergency_enabled(bool is_enabled);
extern void srmdrive_controller_set_torque(double val_pu);
extern void srmdrive_controller_set_speed(double val);

// crd600
extern void crd600_set_power_enabled(bool is_enabled);
extern void crd600_set_drive1_run_enabled(bool is_enabled);
extern void crd600_set_drive2_run_enabled(bool is_enabled);
extern void crd600_set_emergency_enabled(bool is_enabled);
extern void crd600_set_drive1_speed_ref(double val);
extern void crd600_set_drive2_speed_ref(double val);
extern void crd600_set_drive1_torque_ref(double val_pu);
extern void crd600_set_drive2_torque_ref(double val_pu);

extern void crd600_tpdo1_get_drive1_state(char* retbuf, size_t bufsize);
extern void crd600_tpdo1_get_drive2_state(char* retbuf, size_t bufsize);
extern void crd600_tpdo1_get_drive1_ref(char* retbuf, size_t bufsize);
extern void crd600_tpdo1_get_drive2_ref(char* retbuf, size_t bufsize);
extern bool crd600_tpdo1_get_drive1_run();
extern bool crd600_tpdo1_get_drive2_run();
extern bool crd600_tpdo1_get_error();
extern bool crd600_tpdo1_get_warning();
extern bool crd600_tpdo1_get_overheat();
extern void crd600_tpdo1_get_control_loop_type(char* retbuf, size_t bufsize);

/// bmsmain21
extern double bmsmain_tpdo1_get_voltage();
extern double bmsmain_tpdo1_get_current();
extern double bmsmain_tpdo1_get_charge();
extern double bmsmain_tpdo1_get_temp_max();
extern double bmsmain_tpdo1_get_temp_min();

/// launchpad
extern void launchpad_set_client_value(uint tpdo_num, double value);
extern void launchpad_set_server_value(uint rpdo_num, double value);

/// ucanopen_devices
extern size_t ucanopen_devices_get_error_names(string server_name, char** bufsize, size_t str_count, size_t str_size);
extern size_t ucanopen_devices_get_warning_names(string server_name, char** bufsize, size_t str_count, size_t str_size);
extern uint ucanopen_devices_get_error_code(string server_name);
extern uint ucanopen_devices_get_warning_code(string server_name);


int main(string[] args)
{
	var app = new CanMonitor.Application();
	return app.run(args);
}


