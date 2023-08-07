namespace AtvVcu {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcu/atvvcu_motorcontrol.ui")]
public class MotorControl : Adw.Bin {
    [GtkChild]
    private unowned Adw.PreferencesGroup _group;

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

    public MotorControl() {}

    construct {
        ctlmode_comborow.notify["selected"].connect((s, p) => {
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "control_mode", ctlmode_comborow.selected.to_string());
        });

        enable_switch.notify["state"].connect((s, p) => {
            if (enable_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "enable_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "enable_status", "0");
            }
        });

        activedischarge_switch.notify["state"].connect((s, p) => {
            if (activedischarge_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "active_discharge_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "active_discharge_status", "0");
            }
        });

        mainrelay_switch.notify["state"].connect((s, p) => {
            if (mainrelay_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "mainrelay_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "mainrelay_status", "0");
            }
        });

        footbrake_switch.notify["state"].connect((s, p) => {
            if (footbrake_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "footbrake_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "footbrake_status", "0");
            }
        });

        handbrake_switch.notify["state"].connect((s, p) => {
            if (handbrake_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "handbrake_status", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "handbrake_status", "0");
            }
        });

        neutral_button.toggled.connect((s) => {
			if (neutral_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "gear", "0");
			}
		});
		forward_button.toggled.connect((s) => {
			if (forward_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "gear", "1");
			}
		});

		reverse_button.toggled.connect((s) => {
			if (reverse_button.active)
			{
				ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "gear", "2"); 
			}
		});

        speed_lf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_lf_slider.value + 10000)) << 4;
            val = val + 0;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "speed_ref", val.to_string()); 
        });

        speed_lb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_lb_slider.value + 10000)) << 4;
            val = val + 1;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "speed_ref", val.to_string()); 
        });

        speed_rf_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_rf_slider.value + 10000)) << 4;
            val = val + 2;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "speed_ref", val.to_string()); 
        });

        speed_rb_slider.adjustment->value_changed.connect(() => {
            uint val = ((uint)(speed_rb_slider.value + 10000)) << 4;
            val = val + 3;
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "motordrives", "speed_ref", val.to_string()); 
        });
    }


    public string title {
        get { return _group.title; }
        set { _group.title = value; }
    }
}

}
