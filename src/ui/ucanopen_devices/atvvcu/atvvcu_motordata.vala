namespace AtvVcu {

[GtkTemplate (ui = "/src/ui/assets/ucanopen_devices/atvvcu/atvvcu_motordata.ui")]
public class MotorData : Adw.Bin {
    [GtkChild]
    private unowned BoolEntry entry_controlmode_lf;
    [GtkChild]
    private unowned BoolEntry entry_enablestatus_lf;
    [GtkChild]
    private unowned BasicEntry entry_dischargestatus_lf;
    [GtkChild]
    private unowned BasicEntry entry_faultlevel_lf;
    [GtkChild]
    private unowned BasicEntry entry_faultcode_lf;
    [GtkChild]
    private unowned BasicEntry entry_speed_lf;
    [GtkChild]
    private unowned BasicEntry entry_torque_lf;
    [GtkChild]
    private unowned BasicEntry entry_vdc_lf;
    [GtkChild]
    private unowned BasicEntry entry_ac_lf;
    [GtkChild]
    private unowned BasicEntry entry_motortemp_lf;
    [GtkChild]
    private unowned BasicEntry entry_controllertemp_lf;

    [GtkChild]
    private unowned BoolEntry entry_controlmode_lb;
    [GtkChild]
    private unowned BoolEntry entry_enablestatus_lb;
    [GtkChild]
    private unowned BasicEntry entry_dischargestatus_lb;
    [GtkChild]
    private unowned BasicEntry entry_faultlevel_lb;
    [GtkChild]
    private unowned BasicEntry entry_faultcode_lb;
    [GtkChild]
    private unowned BasicEntry entry_speed_lb;
    [GtkChild]
    private unowned BasicEntry entry_torque_lb;
    [GtkChild]
    private unowned BasicEntry entry_vdc_lb;
    [GtkChild]
    private unowned BasicEntry entry_ac_lb;
    [GtkChild]
    private unowned BasicEntry entry_motortemp_lb;
    [GtkChild]
    private unowned BasicEntry entry_controllertemp_lb;

    [GtkChild]
    private unowned BoolEntry entry_controlmode_rf;
    [GtkChild]
    private unowned BoolEntry entry_enablestatus_rf;
    [GtkChild]
    private unowned BasicEntry entry_dischargestatus_rf;
    [GtkChild]
    private unowned BasicEntry entry_faultlevel_rf;
    [GtkChild]
    private unowned BasicEntry entry_faultcode_rf;
    [GtkChild]
    private unowned BasicEntry entry_speed_rf;
    [GtkChild]
    private unowned BasicEntry entry_torque_rf;
    [GtkChild]
    private unowned BasicEntry entry_vdc_rf;
    [GtkChild]
    private unowned BasicEntry entry_ac_rf;
    [GtkChild]
    private unowned BasicEntry entry_motortemp_rf;
    [GtkChild]
    private unowned BasicEntry entry_controllertemp_rf;

    [GtkChild]
    private unowned BoolEntry entry_controlmode_rb;
    [GtkChild]
    private unowned BoolEntry entry_enablestatus_rb;
    [GtkChild]
    private unowned BasicEntry entry_dischargestatus_rb;
    [GtkChild]
    private unowned BasicEntry entry_faultlevel_rb;
    [GtkChild]
    private unowned BasicEntry entry_faultcode_rb;
    [GtkChild]
    private unowned BasicEntry entry_speed_rb;
    [GtkChild]
    private unowned BasicEntry entry_torque_rb;
    [GtkChild]
    private unowned BasicEntry entry_vdc_rb;
    [GtkChild]
    private unowned BasicEntry entry_ac_rb;
    [GtkChild]
    private unowned BasicEntry entry_motortemp_rb;
    [GtkChild]
    private unowned BasicEntry entry_controllertemp_rb;


    public MotorData() {}

    construct {
        Timeout.add(50, update);
    }

    public bool update() {
        update_lf_data();
        update_lb_data();
        update_rf_data();
        update_rb_data();
        return true;
    }

    private void update_lf_data() {
        uint status = ucanopen_server_get_watch_value_u32(Backend.Ucanopen.server, "motordrive", "status_lf");
        entry_controlmode_lf.value = (bool)(status & 0x01);
        entry_enablestatus_lf.value = (bool)(status & 0x02);
        uint discharge_status = (status & 0x0C) >> 2;
        switch (discharge_status) {
        case 0:
            entry_dischargestatus_lf.string_value = "charged";
            break;
        case 1:
        case 2:
            entry_dischargestatus_lf.string_value = "discharged";
            break;
        case 3:
            entry_dischargestatus_lf.string_value = "failed";
            break;
        }
        entry_faultlevel_lf.uint_value = (status & 0xF0) >> 4;
        entry_faultcode_lf.uint_value = (status >> 8) & 0xFF;
        entry_speed_lf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "speed_lf");
        entry_torque_lf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "torque_lf");
        entry_vdc_lf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "vdc_lf");
        entry_ac_lf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "ac_lf");
        entry_motortemp_lf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "motortemp_lf");
        entry_controllertemp_lf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "conttemp_lf");
    }

    private void update_lb_data() {
        uint status = ucanopen_server_get_watch_value_u32(Backend.Ucanopen.server, "motordrive", "status_lb");
        entry_controlmode_lb.value = (bool)(status & 0x01);
        entry_enablestatus_lb.value = (bool)(status & 0x02);
        uint discharge_status = (status & 0x0C) >> 2;
        switch (discharge_status) {
        case 0:
            entry_dischargestatus_lb.string_value = "charged";
            break;
        case 1:
        case 2:
            entry_dischargestatus_lb.string_value = "discharged";
            break;
        case 3:
            entry_dischargestatus_lb.string_value = "failed";
            break;
        }
        entry_faultlevel_lb.uint_value = (status & 0xF0) >> 4;
        entry_faultcode_lb.uint_value = (status >> 8) & 0xFF;
        entry_speed_lb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "speed_lb");
        entry_torque_lb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "torque_lb");
        entry_vdc_lb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "vdc_lb");
        entry_ac_lb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "ac_lb");
        entry_motortemp_lb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "motortemp_lb");
        entry_controllertemp_lb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "conttemp_lb");
    }
    
    private void update_rf_data() {
        uint status = ucanopen_server_get_watch_value_u32(Backend.Ucanopen.server, "motordrive", "status_rf");
        entry_controlmode_rf.value = (bool)(status & 0x01);
        entry_enablestatus_rf.value = (bool)(status & 0x02);
        uint discharge_status = (status & 0x0C) >> 2;
        switch (discharge_status) {
        case 0:
            entry_dischargestatus_rf.string_value = "charged";
            break;
        case 1:
        case 2:
            entry_dischargestatus_rf.string_value = "discharged";
            break;
        case 3:
            entry_dischargestatus_rf.string_value = "failed";
            break;
        }
        entry_faultlevel_rf.uint_value = (status & 0xF0) >> 4;
        entry_faultcode_rf.uint_value = (status >> 8) & 0xFF;
        entry_speed_rf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "speed_rf");
        entry_torque_rf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "torque_rf");
        entry_vdc_rf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "vdc_rf");
        entry_ac_rf.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "ac_rf");
        entry_motortemp_rf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "motortemp_rf");
        entry_controllertemp_rf.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "conttemp_rf");
    }
    
    private void update_rb_data() {
        uint status = ucanopen_server_get_watch_value_u32(Backend.Ucanopen.server, "motordrive", "status_rb");
        entry_controlmode_rb.value = (bool)(status & 0x01);
        entry_enablestatus_rb.value = (bool)(status & 0x02);
        uint discharge_status = (status & 0x0C) >> 2;
        switch (discharge_status) {
        case 0:
            entry_dischargestatus_rb.string_value = "charged";
            break;
        case 1:
        case 2:
            entry_dischargestatus_rb.string_value = "discharged";
            break;
        case 3:
            entry_dischargestatus_rb.string_value = "failed";
            break;
        }
        entry_faultlevel_rb.uint_value = (status & 0xF0) >> 4;
        entry_faultcode_rb.uint_value = (status >> 8) & 0xFF;
        entry_speed_rb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "speed_rb");
        entry_torque_rb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "torque_rb");
        entry_vdc_rb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "vdc_rb");
        entry_ac_rb.float_value = ucanopen_server_get_watch_value_f32(Backend.Ucanopen.server, "motordrive", "ac_rb");
        entry_motortemp_rb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "motortemp_rb");
        entry_controllertemp_rb.int_value = ucanopen_server_get_watch_value_i32(Backend.Ucanopen.server, "motordrive", "conttemp_rb");
    }
}    

}
