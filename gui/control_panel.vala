///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/control_panel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switchWakeUp;

	[GtkChild]
	private unowned Gtk.Switch switchHV;

	[GtkChild]
	private unowned Gtk.Switch switchRelay;

	[GtkChild]
	private unowned Gtk.Switch switchEmergency;

	[GtkChild]
	private unowned Gtk.CheckButton buttonParking;

	[GtkChild]
	private unowned Gtk.CheckButton buttonReverse;

	[GtkChild]
	private unowned Gtk.CheckButton buttonNeutral;

	[GtkChild]
	private unowned Gtk.CheckButton buttonDrive;

	[GtkChild]
	private unowned Gtk.Switch switchEcoMode;

	[GtkChild]
	private unowned SliderWithText sliderTorque;

	public ControlPanel() {}

	construct
	{
		switchWakeUp.notify["state"].connect((s, p) => {
				atv_vcm_set_wakeup_state(switchWakeUp.state);
		});

		switchHV.notify["state"].connect((s, p) => {
				atv_vcm_set_hv_power_supply(switchHV.state);
		});

		switchRelay.notify["state"].connect((s, p) => {
				atv_vcm_set_relay_plus_output(switchRelay.state);
		});

		buttonParking.toggled.connect((s) => { atv_gear_selector_set_gear(0); });
		buttonReverse.toggled.connect((s) => { atv_gear_selector_set_gear(1); });
		buttonNeutral.toggled.connect((s) => { atv_gear_selector_set_gear(2); });
		buttonDrive.toggled.connect((s) => { atv_gear_selector_set_gear(3); });

		switchEcoMode.notify["state"].connect((s, p) => {
				atv_gear_selector_set_ecomode(switchRelay.state);
		});

		sliderTorque.adjustment->notify["value"].connect((s, p) => {
				atv_vcm_set_torque(sliderTorque.value);
		});
	}
}
	
	
}


