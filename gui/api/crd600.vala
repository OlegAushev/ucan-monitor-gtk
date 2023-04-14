///
///
///


extern void crd600_set_power_enabled(bool enabled);
extern void crd600_set_drive1_run_enabled(bool enabled);
extern void crd600_set_drive2_run_enabled(bool enabled);
extern void crd600_set_emergency_enabled(bool enabled);
extern void crd600_set_drive1_speed_ref(double val);
extern void crd600_set_drive2_speed_ref(double val);
extern void crd600_set_drive1_torque_ref(double val_pu);
extern void crd600_set_drive2_torque_ref(double val_pu);

extern void crd600_tpdo1_get_drive1_state(char* retbuf, int bufsize);
extern void crd600_tpdo1_get_drive2_state(char* retbuf, int bufsize);
extern void crd600_tpdo1_get_drive1_ref(char* retbuf, int bufsize);
extern void crd600_tpdo1_get_drive2_ref(char* retbuf, int bufsize);
extern bool crd600_tpdo1_get_drive1_run();
extern bool crd600_tpdo1_get_drive2_run();
extern bool crd600_tpdo1_get_error();
extern bool crd600_tpdo1_get_warning();
extern bool crd600_tpdo1_get_overheat();
extern void crd600_tpdo1_get_control_loop_type(char* retbuf, int bufsize);