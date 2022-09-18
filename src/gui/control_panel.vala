///
///
///


extern void motordrive_controller_set_power_state(bool state);


namespace CanMonitor {


[GtkTemplate (ui = "/src/gui/control_panel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switchPower;

	public ControlPanel()
	{
		
	}

	construct
	{
		switchPower.notify["state"].connect((s, p) => {
			motordrive_controller_set_power_state(switchPower.state);
		});
	}
}
	
	
}


