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
	private unowned Gtk.Button button_reset;
	[GtkChild]
	private unowned SpinButtonScale slider_speed;
	[GtkChild]
	private unowned SpinButtonScale slider_torque;
	[GtkChild]
	private unowned Gtk.Switch switch_emergency;

	public ControlPanel() {}

	construct
	{
		switch_power.notify["state"].connect((s, p) => {
			srmdrive_controller_set_power_enabled(switch_power.state);
		});

		switch_run.notify["state"].connect((s, p) => {
			srmdrive_controller_set_run_enabled(switch_run.state);
		});

		button_calibrate.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "BEGIN POSITION SENSOR CALIBRATION");
		});

		button_invert.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "INVERT ROTATION");
		});

		button_reset.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "RESET DEVICE");
		});
		
		slider_speed.adjustment->value_changed.connect(() => {
			srmdrive_controller_set_speed(slider_speed.value);
		});
		
		slider_torque.adjustment->value_changed.connect(() => {
			srmdrive_controller_set_torque(slider_torque.value / 100.0);
		});

		switch_emergency.notify["state"].connect((s, p) => {
			srmdrive_controller_set_emergency_enabled(switch_emergency.state);
		});
	}
}
	
	
}


