///
///
///


extern void srmdrive_set_power_enabled(bool enabled);
extern void srmdrive_set_run_enabled(bool enabled);
extern void srmdrive_set_emergency_enabled(bool enabled);
extern void srmdrive_set_torque(double value_perunit);
extern void srmdrive_set_speed(double value_rpm);

extern void srmdrive_tpdo1_get_drive_state(char* retbuf, size_t bufsize);