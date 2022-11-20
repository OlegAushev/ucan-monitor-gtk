///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/control_panel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switchHV;

	[GtkChild]
	private unowned Gtk.Switch switchRelay;

	[GtkChild]
	private unowned Gtk.Switch switchEmergency;

	//[GtkChild]
	//private unowned SliderWithText sliderSpeed;

	[GtkChild]
	private unowned SliderWithText sliderTorque;

	public ControlPanel() {}

	construct
	{
		switchHV.notify["state"].connect((s, p) => {
				atv_vcm_set_hv_power_supply(switchHV.state);
		});

		switchRelay.notify["state"].connect((s, p) => {
				atv_vcm_set_relay_plus_output(switchRelay.state);
		});

		//  switchEmergency.notify["state"].connect((s, p) => {
		//  		srmdrive_controller_set_emergency_enabled(switchEmergency.state);
		//  });

		//  sliderSpeed.adjustment->notify["value"].connect((s, p) => {
		//  		srmdrive_controller_set_speed(sliderSpeed.value);
		//  });

		sliderTorque.adjustment->notify["value"].connect((s, p) => {
				atv_vcm_set_torque(sliderTorque.value);
		});
	}
}
	
	
}


