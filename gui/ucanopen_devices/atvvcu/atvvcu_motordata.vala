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
        string buf = string.nfill(16, '\0');
        ucanopen_server_get_watch_value(Backend.Ucanopen.server, "motordrive", "speed_lf", buf, 16);
        entry_speed_lf.string_value = buf;
    }

    private void update_lb_data() {

    }
    
    private void update_rf_data() {

    }
    
    private void update_rb_data() {

    }
}    

}
