///
///
///


extern void motordrive_controller_set_power_state(bool state);
extern void motordrive_controller_set_run_state(bool state);
extern void motordrive_controller_set_emergency_state(bool state);


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

	public ControlPanel()
	{
		
	}

	construct
	{
		switchPower.notify["state"].connect((s, p) => {
			motordrive_controller_set_power_state(switchPower.state);
		});

		switchRun.notify["state"].connect((s, p) => {
			motordrive_controller_set_run_state(switchRun.state);
		});

		switchEmergency.notify["state"].connect((s, p) => {
			motordrive_controller_set_emergency_state(switchEmergency.state);
		});
	}
}
	
	
}


