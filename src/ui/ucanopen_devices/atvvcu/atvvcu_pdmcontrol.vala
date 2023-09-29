namespace AtvVcu {

[GtkTemplate (ui = "/src/ui/assets/ucanopen_devices/atvvcu/atvvcu_pdmcontrol.ui")]
public class PdmControl : Adw.Bin {
    [GtkChild]
    private unowned Gtk.Switch battery_switch;
    [GtkChild]
    private unowned Gtk.Switch motor1bypass_switch;
    [GtkChild]
    private unowned Gtk.Switch motor2bypass_switch;
    [GtkChild]
    private unowned Gtk.Switch motor3bypass_switch;
    [GtkChild]
    private unowned Gtk.Switch motor4bypass_switch;
    [GtkChild]
    private unowned Gtk.Switch chargemode_switch;
    [GtkChild]
    private unowned Gtk.Switch allowcharge_switch;
    [GtkChild]
    private unowned Gtk.Switch auxequipbypass_switch;

    public PdmControl() {}

    construct {
        battery_switch.notify["state"].connect((s, p) => {
            if (battery_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "0");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "0");
            }
        });

        motor1bypass_switch.notify["state"].connect((s, p) => {
            if (motor1bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "1");
            }
        });

        motor2bypass_switch.notify["state"].connect((s, p) => {
            if (motor2bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "2");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "2");
            }
        });

        motor3bypass_switch.notify["state"].connect((s, p) => {
            if (motor3bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "3");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "3");
            }
        });

        motor4bypass_switch.notify["state"].connect((s, p) => {
            if (motor4bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "4");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "4");
            }
        });

        chargemode_switch.notify["state"].connect((s, p) => {
            if (chargemode_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "5");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "5");
            }
        });

        allowcharge_switch.notify["state"].connect((s, p) => {
            if (allowcharge_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "6");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "6");
            }
        });

        auxequipbypass_switch.notify["state"].connect((s, p) => {
            if (auxequipbypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "enable_contactor", "7");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "ctl", "pdm", "disable_contactor", "7");
            }
        });
    }
}

}
