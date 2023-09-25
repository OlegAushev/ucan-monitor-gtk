namespace SrmDrive {

[GtkTemplate (ui = "/src/gui/assets/ucanopen_devices/srmdrive/srmdrive_controlpanel.ui")]
public class ControlPanel : Adw.Bin {
    [GtkChild]
    private unowned Gtk.Switch power_switch;
    [GtkChild]
    private unowned Gtk.Switch run_switch;

    [GtkChild]
    private unowned Gtk.CheckButton ctlmode_torque_button;
    [GtkChild]
    private unowned Gtk.CheckButton ctlmode_speed_button;

    [GtkChild]
    private unowned Gtk.Button calibrate_button;
    [GtkChild]
    private unowned Gtk.Button invert_button;
    [GtkChild]
    private unowned Gtk.Button resetdriverfault_button;
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

    construct {
        power_switch.notify["state"].connect((s, p) => {
            srmdrive_set_power_enabled(power_switch.state);
        });

        run_switch.notify["state"].connect((s, p) => {
            srmdrive_set_run_enabled(run_switch.state);
        });

        ctlmode_torque_button.toggled.connect(() => {
            if (ctlmode_torque_button.active) {
                srmdrive_set_ctlmode(0);
            }
        });

        ctlmode_speed_button.toggled.connect(() => {
            if (ctlmode_speed_button.active) {
                srmdrive_set_ctlmode(1);
            }
        });

        calibrate_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "ctl", "drive", "calibrate");
        });

        invert_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "ctl", "drive", "invert_rotdir");
        });

        resetdriverfault_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "ctl", "drive", "reset_driver_fault");
        });

        clearerrors_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "ctl", "sys", "clear_errors");
        });

        resetdevice_button.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "ctl", "sys", "reset_device");
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
            srmdrive_set_manual_fieldctl_enabled(field_expanderrow.enable_expansion);
        });

        field_slider.adjustment->value_changed.connect(() => {
            srmdrive_set_field_current(field_slider.value);
        });

        //--------------------------------------------------------------------------------------------------------------
        openloop_expanderrow.notify["enable-expansion"].connect((s,p) => {
            srmdrive_set_ctlloop((int)openloop_expanderrow.enable_expansion);
        });

        current_slider.adjustment->value_changed.connect(() => {
            srmdrive_set_stator_current(current_slider.value / 100.0);
        });

        //--------------------------------------------------------------------------------------------------------------
        gammacorrection_slider.adjustment->value_changed.connect(() => {
            ucanopen_server_write(Backend.Ucanopen.server, "ctl", "drive", "set_gamma_correction", gammacorrection_slider.value.to_string());
        });

        //--------------------------------------------------------------------------------------------------------------
        emergency_switch.notify["state"].connect((s, p) => {
            srmdrive_set_emergency_enabled(emergency_switch.state);
        });     
    }
}

}
