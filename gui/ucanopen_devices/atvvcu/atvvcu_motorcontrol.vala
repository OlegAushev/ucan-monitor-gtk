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
    }


    public string title {
        get { return _group.title; }
        set { _group.title = value; }
    }
}

}
