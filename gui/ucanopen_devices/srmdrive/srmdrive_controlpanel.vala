///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/ucanopen_devices/srmdrive/srmdrive_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switch_power;
	[GtkChild]
	private unowned Gtk.Switch switch_run;
	[GtkChild]
	private unowned Gtk.Button button_calibrate;
	[GtkChild]
	private unowned Gtk.Button button_invert;
    [GtkChild]
	private unowned Gtk.Button button_clear_errors;
	[GtkChild]
	private unowned Gtk.Button button_reset_device;

	[GtkChild]
	private unowned SpinButtonScale slider_speed;
	[GtkChild]
	private unowned SpinButtonScale slider_torque;

	[GtkChild]
	private unowned Adw.ExpanderRow expanderrow_field;
    [GtkChild]
	private unowned SpinButtonScale slider_field;

	[GtkChild]
	private unowned Gtk.Switch switch_emergency;

	public ControlPanel() {}

	construct
	{
		switch_power.notify["state"].connect((s, p) => {
			srmdrive_set_power_enabled(switch_power.state);
		});

		switch_run.notify["state"].connect((s, p) => {
			srmdrive_set_run_enabled(switch_run.state);
		});

		button_calibrate.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "calibrate");
		});

		button_invert.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "invert_rotdir");
		});

        button_clear_errors.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "clear_errors");
        });

		button_reset_device.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "reset_device");
		});
		
        //--------------------------------------------------------------------------------------------------------------
		slider_speed.adjustment->value_changed.connect(() => {
			srmdrive_set_speed(slider_speed.value);
		});
		
		slider_torque.adjustment->value_changed.connect(() => {
			srmdrive_set_torque(slider_torque.value / 100.0);
		});

        //--------------------------------------------------------------------------------------------------------------
        expanderrow_field.notify["enable-expansion"].connect((s,p) => {
            if (expanderrow_field.enable_expansion) {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "enable_manual_field");
            } else {
                ucanopen_server_exec(Backend.Ucanopen.server, "drive", "ctl", "disable_manual_field");
            }
		});

        slider_field.adjustment->value_changed.connect(() => {
            ucanopen_server_write(Backend.Ucanopen.server, "drive", "ctl", "set_field_current", slider_field.value.to_string());
        });

        //--------------------------------------------------------------------------------------------------------------
		switch_emergency.notify["state"].connect((s, p) => {
			srmdrive_set_emergency_enabled(switch_emergency.state);
		});

        
	}
}
	
	
}


