///
///
///


extern void srmdrive_set_power_enabled(bool enabled);
extern void srmdrive_set_run_enabled(bool enabled);
extern void srmdrive_set_emergency_enabled(bool enabled);
extern void srmdrive_set_torque(double value_perunit);
extern void srmdrive_set_speed(double value_rpm);

extern void srmdrive_tpdo1_get_drive_state(char* retbuf, size_t bufsize);
extern bool srmdrive_tpdo1_get_run_status();
extern bool srmdrive_tpdo1_get_error_status();
extern bool srmdrive_tpdo1_get_warning_status();
extern bool srmdrive_tpdo1_get_overheat_status();
extern void srmdrive_tpdo1_get_drive_reference(char* retbuf, size_t bufsize);
extern void srmdrive_tpdo1_get_drive_loop_type(char* retbuf, size_t bufsize);
extern void srmdrive_tpdo1_get_dc_voltage(char* retbuf, size_t bufsize);
extern void srmdrive_tpdo1_get_torque(char* retbuf, size_t bufsize);
extern void srmdrive_tpdo1_get_speed(char* retbuf, size_t bufsize);

