///
///
///


extern void motordrive_controller_set_power_enabled(bool isEnabled);
extern void motordrive_controller_set_run_enabled(bool isEnabled);
extern void motordrive_controller_set_emergency_enabled(bool isEnabled);
extern void motordrive_controller_set_torque(double valPu);
extern void motordrive_controller_set_speed(double val);


namespace CanMonitor {


[GtkTemplate (ui = "/src/gui/control_panel.ui")]
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

	public ControlPanel()
	{
		
	}

	construct
	{
		switchPower.notify["state"].connect((s, p) => {
				motordrive_controller_set_power_enabled(switchPower.state);
		});

		switchRun.notify["state"].connect((s, p) => {
				motordrive_controller_set_run_enabled(switchRun.state);
		});

		switchEmergency.notify["state"].connect((s, p) => {
				motordrive_controller_set_emergency_enabled(switchEmergency.state);
		});

		sliderSpeed.adjustment->notify["value"].connect((s, p) => {
				motordrive_controller_set_speed(sliderSpeed.value);
		});

		sliderTorque.adjustment->notify["value"].connect((s, p) => {
				motordrive_controller_set_torque(sliderTorque.value / 100.0);
		});
	}
}
	
	
}


