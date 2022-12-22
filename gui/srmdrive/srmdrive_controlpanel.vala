///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/srmdrive/srmdrive_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switchPower;
	[GtkChild]
	private unowned Gtk.Switch switchRun;
	[GtkChild]
	private unowned Gtk.Button buttonCalibrate;
	[GtkChild]
	private unowned Gtk.Button buttonInvert;
	[GtkChild]
	private unowned Gtk.Button buttonReset;
	[GtkChild]
	private unowned SpinButtonScale sliderSpeed;
	[GtkChild]
	private unowned SpinButtonScale sliderTorque;
	[GtkChild]
	private unowned Gtk.Switch switchEmergency;

	public ControlPanel() {}

	construct
	{
		switchPower.notify["state"].connect((s, p) => {
			srmdrive_controller_set_power_enabled(switchPower.state);
		});

		switchRun.notify["state"].connect((s, p) => {
			srmdrive_controller_set_run_enabled(switchRun.state);
		});

		buttonCalibrate.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "BEGIN POSITION SENSOR CALIBRATION");
		});

		buttonInvert.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "INVERT ROTATION");
		});

		buttonReset.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "RESET DEVICE");
		});
		
		sliderSpeed.adjustment->value_changed.connect(() => {
			srmdrive_controller_set_speed(sliderSpeed.value);
		});
		
		sliderTorque.adjustment->value_changed.connect(() => {
			srmdrive_controller_set_torque(sliderTorque.value / 100.0);
		});

		switchEmergency.notify["state"].connect((s, p) => {
			srmdrive_controller_set_emergency_enabled(switchEmergency.state);
		});
	}
}
	
	
}


