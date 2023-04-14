///
///
///


extern void srmdrive_set_power_enabled(bool enabled);
extern void srmdrive_set_run_enabled(bool enabled);
extern void srmdrive_set_emergency_enabled(bool enabled);
extern void srmdrive_set_torque(double value_perunit);
extern void srmdrive_set_speed(double value_rpm);

extern void srmdrive_tpdo1_get_drive_state(char* retbuf, int bufsize);
extern bool srmdrive_tpdo1_get_run_status();
extern bool srmdrive_tpdo1_get_error_status();
extern bool srmdrive_tpdo1_get_warning_status();
extern bool srmdrive_tpdo1_get_overheat_status();
extern void srmdrive_tpdo1_get_drive_reference(char* retbuf, int bufsize);
extern void srmdrive_tpdo1_get_drive_loop_type(char* retbuf, int bufsize);
extern uint srmdrive_tpdo1_get_dc_voltage();
extern int srmdrive_tpdo1_get_torque();
extern int srmdrive_tpdo1_get_speed();

extern uint srmdrive_tpdo2_get_stator_current();
extern uint srmdrive_tpdo2_get_field_current();
extern uint srmdrive_tpdo2_get_out_voltage();
extern uint srmdrive_tpdo2_get_mech_power();
extern uint srmdrive_tpdo2_get_elec_power();

extern int srmdrive_tpdo3_get_pwrmodule_temp();
extern int srmdrive_tpdo3_get_excmodule_temp();
extern int srmdrive_tpdo3_get_pcb_temp();
extern int srmdrive_tpdo3_get_aw_temp();
extern int srmdrive_tpdo3_get_fw_temp();

