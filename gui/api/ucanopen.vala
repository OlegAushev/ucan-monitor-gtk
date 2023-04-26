///
///
///


extern int cansocket_connect(string interface, int bitrate);
extern int cansocket_disconnect();

extern void ucanopen_client_set_node_id(uint node_id);
extern void ucanopen_client_set_server_id(string server_name ,uint node_id);
extern void ucanopen_client_set_tpdo_enabled(bool enabled);
extern void ucanopen_client_set_server_rpdo_enabled(bool enabled);
extern void ucanopen_client_set_sync_enabled(bool enabled);
extern void ucanopen_client_set_sync_period(int period);
extern void ucanopen_client_set_watch_enabled(bool enabled);
extern void ucanopen_client_set_watch_period(int period);
extern void ucanopen_server_get_watch_value(string server_name, string watch_name, char* retbuf, int bufsize);
extern int ucanopen_server_get_config_categories(string server_name, char** retbuf, int str_count, int str_size);
extern int ucanopen_server_get_config_objects(string server_name, string category, char** retbuf, int str_count, int str_size);
extern bool ucanopen_server_is_heartbeat_ok(string server_name);
extern void ucanopen_server_get_nmt_state(string server_name, char* retbuf, int bufsize);
extern bool ucanopen_server_is_tpdo_ok(string server_name, uint tpdo_num);
extern ulong ucanopen_server_get_tpdo_data(string server_name, uint tpdo_num);
extern void ucanopen_server_read(string server_name, string category, string subcategory, string name);
extern void ucanopen_server_write(string server_name, string category, string subcategory, string name, string value);
extern void ucanopen_server_exec(string server_name, string category, string subcategory, string name);

extern void ucanopen_server_read_string(string server_name, string category, string subcategory, string name, uint timeout_ms, char* retbuf, int bufsize);
extern void ucanopen_server_read_numval(string server_name, string category, string subcategory, string name, uint timeout_ms, char* retbuf, int bufsize);


extern int ucanopen_devices_get_error_names(string server_name, char** bufsize, int str_count, int str_size);
extern int ucanopen_devices_get_warning_names(string server_name, char** bufsize, int str_count, int str_size);
extern uint ucanopen_devices_get_error_code(string server_name);
extern uint ucanopen_devices_get_warning_code(string server_name);

