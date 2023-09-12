namespace AtvVcu {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcu/atvvcu_motorcontrol.ui")]
public class MotorControl : Adw.Bin {
    [GtkChild]
    private unowned Adw.PreferencesGroup _group;

    [GtkChild]
    private unowned Gtk.Switch lf_enable_node_switch;
    [GtkChild]
    private unowned Gtk.Switch lb_enable_node_switch;
    [GtkChild]
    private unowned Gtk.Switch rf_enable_node_switch;
    [GtkChild]
    private unowned Gtk.Switch rb_enable_node_switch;

    [GtkChild]
    private unowned Adw.ComboRow ctlmode_comborow;
    [GtkChild]
    private unowned Gtk.StringList ctlmode_list;

    [GtkChild]
    private unowned Gtk.Switch enable_switch;
    [GtkChild]
    private unowned Gtk.Switch activedischarge_switch;
    [GtkChild]
    private unowned Gtk.Switch mainrelay_switch;
    [GtkChild]
    private unowned Gtk.Switch footbrake_switch;
    [GtkChild]
    private unowned Gtk.Switch handbrake_switch;
    [GtkChild]
    private unowned Gtk.Switch faultreset_switch;
    
    [GtkChild]
    private unowned Gtk.CheckButton neutral_button;
    [GtkChild]
    private unowned Gtk.CheckButton forward_button;
    [GtkChild]
    private unowned Gtk.CheckButton reverse_button;

    [GtkChild]
    private unowned SpinButtonScale speed_lf_slider;
    [GtkChild]
    private unowned SpinButtonScale speed_lb_slider;
    [GtkChild]
    private unowned SpinButtonScale speed_rf_slider;
    [GtkChild]
    private unowned SpinButtonScale speed_rb_slider;

    [GtkChild]
    private unowned SpinButtonScale torque_lf_slider;
    [GtkChild]
    private unowned SpinButtonScale torque_lb_slider;
    [GtkChild]
    private unowned SpinButtonScale torque_rf_slider;
    [GtkChild]
    private unowned SpinButtonScale torque_rb_slider;

    [GtkChild]
    private unowned SpinButtonScale eltorque_max_slider;
    [GtkChild]
    private unowned SpinButtonScale braketorque_max_slider;

    public MotorControl() {}

    construct {
        lf_enable_node_switch.notify["state"].connect((s, p) => {
            if (lf_enable_node_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_node", "0");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "disable_node", "0");
            }
        });

        lb_enable_node_switch.notify["state"].connect((s, p) => {
            if (lb_enable_node_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_node", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "disable_node", "1");
            }
        });

        rf_enable_node_switch.notify["state"].connect((s, p) => {
            if (rf_enable_node_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_node", "2");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "disable_node", "2");
            }
        });

        rb_enable_node_switch.notify["state"].connect((s, p) => {
            if (rb_enable_node_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_node", "3");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "disable_node", "3");
            }
        });

























        ctlmode_comborow.notify["selected"].connect((s, p) => {
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "control_mode", ctlmode_comborow.selected.to_string());
        });

        enable_switch.notify["state"].connect((s, p) => {
            if (enable_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "enable_status", "0");
            }
        });

        activedischarge_switch.notify["state"].connect((s, p) => {
            if (activedischarge_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "active_discharge_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "active_discharge_status", "0");
            }
        });

        mainrelay_switch.notify["state"].connect((s, p) => {
            if (mainrelay_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "mainrelay_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "mainrelay_status", "0");
            }
        });

        footbrake_switch.notify["state"].connect((s, p) => {
            if (footbrake_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "footbrake_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "footbrake_status", "0");
            }
        });

        handbrake_switch.notify["state"].connect((s, p) => {
            if (handbrake_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "handbrake_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "handbrake_status", "0");
            }
        });

        faultreset_switch.notify["state"].connect((s, p) => {
            if (faultreset_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "faultreset_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "faultreset_status", "0");
            }
        });

        neutral_button.toggled.connect((s) => {
			if (neutral_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "gear", "0");
			}
		});
		forward_button.toggled.connect((s) => {
			if (forward_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "gear", "1");
			}
		});

		reverse_button.toggled.connect((s) => {
			if (reverse_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "gear", "2"); 
			}
		});

        speed_lf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_lf_slider.value + 10000)) << 4;
            val = val + 0;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "speed_ref", val.to_string()); 
        });

        speed_lb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_lb_slider.value + 10000)) << 4;
            val = val + 1;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "speed_ref", val.to_string()); 
        });

        speed_rf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_rf_slider.value + 10000)) << 4;
            val = val + 2;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "speed_ref", val.to_string()); 
        });

        speed_rb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_rb_slider.value + 10000)) << 4;
            val = val + 3;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "speed_ref", val.to_string()); 
        });

        //---

        torque_lf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(torque_lf_slider.value + 400)) << 4;
            val = val + 0;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "torque_ref", val.to_string()); 
        });

        torque_lb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(torque_lb_slider.value + 400)) << 4;
            val = val + 1;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "torque_ref", val.to_string()); 
        });

        torque_rf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(torque_rf_slider.value + 400)) << 4;
            val = val + 2;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "torque_ref", val.to_string()); 
        });

        torque_rb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(torque_rb_slider.value + 400)) << 4;
            val = val + 3;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "torque_ref", val.to_string()); 
        });

        eltorque_max_slider.adjustment->value_changed.connect(() => {
            uint val = (uint)eltorque_max_slider.value;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "eltorque_max", val.to_string()); 
        });

        braketorque_max_slider.adjustment->value_changed.connect(() => {
            uint val = (uint)(braketorque_max_slider.value + 400);
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrive", "braketorque_max", val.to_string()); 
        });
    }


    public string title {
        get { return _group.title; }
        set { _group.title = value; }
    }
}

}
