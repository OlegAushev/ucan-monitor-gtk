namespace AtvVcm {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcm/atvvcm_pdmcontrol.ui")]
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
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "0");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "0");
            }
        });

        motor1bypass_switch.notify["state"].connect((s, p) => {
            if (motor1bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "1");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "1");
            }
        });

        motor2bypass_switch.notify["state"].connect((s, p) => {
            if (motor2bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "2");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "2");
            }
        });

        motor3bypass_switch.notify["state"].connect((s, p) => {
            if (motor3bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "3");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "3");
            }
        });

        motor4bypass_switch.notify["state"].connect((s, p) => {
            if (motor4bypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "4");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "4");
            }
        });

        chargemode_switch.notify["state"].connect((s, p) => {
            if (chargemode_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "5");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "5");
            }
        });

        allowcharge_switch.notify["state"].connect((s, p) => {
            if (allowcharge_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "6");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "6");
            }
        });

        auxequipbypass_switch.notify["state"].connect((s, p) => {
            if (auxequipbypass_switch.state) {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_enable", "7");
            } else {
                ucanopen_server_write(Backend.Ucanopen.server, "pdm", "ctl", "contactor_disable", "7");
            }
        });
    }
}

}

