///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/control_panel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switchPower;

	[GtkChild]
	private unowned Gtk.Switch switchRun;

	[GtkChild]
	private unowned Gtk.Switch switchEmergency;

	[GtkChild]
	private unowned SliderWithText sliderSpeed;

	[GtkChild]
	private unowned SliderWithText sliderTorque;

	public ControlPanel() {}

	construct
	{
		//  switchPower.notify["state"].connect((s, p) => {
		//  		srmdrive_controller_set_power_enabled(switchPower.state);
		//  });

		//  switchRun.notify["state"].connect((s, p) => {
		//  		srmdrive_controller_set_run_enabled(switchRun.state);
		//  });

		//  switchEmergency.notify["state"].connect((s, p) => {
		//  		srmdrive_controller_set_emergency_enabled(switchEmergency.state);
		//  });

		//  sliderSpeed.adjustment->notify["value"].connect((s, p) => {
		//  		srmdrive_controller_set_speed(sliderSpeed.value);
		//  });

		//  sliderTorque.adjustment->notify["value"].connect((s, p) => {
		//  		srmdrive_controller_set_torque(sliderTorque.value / 100.0);
		//  });
	}
}
	
	
}


