///
///
///


namespace SrmDrive {

[GtkTemplate (ui = "/gui/ucanopen_devices/srmdrive/srmdrive_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
    [GtkChild]
    private unowned Gtk.Switch power_switch;
    [GtkChild]
    private unowned Gtk.Switch run_switch;
    [GtkChild]
    private unowned Gtk.Button calibrate_button;
    [GtkChild]
    private unowned Gtk.Button invert_button;
    [GtkChild]
    private unowned Gtk.Button clearerrors_button;
    [GtkChild]
    private unowned Gtk.Button resetdevice_button;

    [GtkChild]
    private unowned SpinButtonScale speed_slider;
    [GtkChild]
    private unowned SpinButtonScale torque_slider;

    [GtkChild]
    private unowned Adw.ExpanderRow field_expanderrow;
    [GtkChild]
    private unowned SpinButtonScale field_slider;

    [GtkChild]
    private unowned Adw.ExpanderRow openloop_expanderrow;
    [GtkChild]
    private unowned SpinButtonScale current_slider;

    [GtkChild]
    private unowned SpinButtonScale gammacorrection_slider;

    [GtkChild]
    private unowned Gtk.Switch emergency_switch;

    public ControlPanel() {}

    construct
    {
        power_switch.notify["state"].connect((s, p) => {
            srmdrive_set_power_enabled(power_switch.state);
        });

        run_switch.notify["state"].connect((s, p) => {
            srmdrive_set_run_enabled(run_switch.state);
        });

        calibrate_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "calibrate");
        });

        invert_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "invert_rotdir");
        });

        clearerrors_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "clear_errors");
        });

        resetdevice_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "reset_device");
        });
        
        //--------------------------------------------------------------------------------------------------------------
        speed_slider.adjustment->value_changed.connect(() => {
            srmdrive_set_speed(speed_slider.value);
        });
        
        torque_slider.adjustment->value_changed.connect(() => {
            srmdrive_set_torque(torque_slider.value / 100.0);
        });

        //--------------------------------------------------------------------------------------------------------------
        field_expanderrow.notify["enable-expansion"].connect((s,p) => {
            if (field_expanderrow.enable_expansion) {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "enable_manual_field");
            } else {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "disable_manual_field");
            }
        });

        field_slider.adjustment->value_changed.connect(() => {
            ucanopen_server_write(Backend.Ucanopen.server, "drive", "ctl", "set_field_current", field_slider.value.to_string());
        });

        //--------------------------------------------------------------------------------------------------------------
        openloop_expanderrow.notify["enable-expansion"].connect((s,p) => {
            if (openloop_expanderrow.enable_expansion) {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "enable_open_loop");
            } else {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "enable_closed_loop");
            }
        });

        current_slider.adjustment->value_changed.connect(() => {
            ucanopen_server_write(Backend.Ucanopen.server, "drive", "ctl", "set_current", current_slider.value.to_string());
        });

        //--------------------------------------------------------------------------------------------------------------
        gammacorrection_slider.adjustment->value_changed.connect(() => {
            ucanopen_server_write(Backend.Ucanopen.server, "drive", "ctl", "set_gamma_correction", gammacorrection_slider.value.to_string());
        });

        //--------------------------------------------------------------------------------------------------------------
        emergency_switch.notify["state"].connect((s, p) => {
            srmdrive_set_emergency_enabled(emergency_switch.state);
        });

        
    }
}

}


