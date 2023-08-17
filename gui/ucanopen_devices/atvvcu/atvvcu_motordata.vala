namespace AtvVcu {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcu/atvvcu_motordata.ui")]
public class MotorData : Adw.Bin {
    [GtkChild]
    private unowned BasicEntry entry_status_lf;
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
    private unowned BasicEntry entry_status_lb;
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
    private unowned BasicEntry entry_status_rf;
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
    private unowned BasicEntry entry_status_rb;
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
        entry_status_lf.uint_value = status & 0xFF;
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
        entry_status_lb.uint_value = status & 0xFF;
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
        entry_status_rf.uint_value = status & 0xFF;
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
        entry_status_rb.uint_value = status & 0xFF;
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
