///
///
///


namespace Crd600 {


[GtkTemplate (ui = "/gui/ucanopen_devices/crd600/crd600_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Switch switch_power;
	[GtkChild]
	private unowned Gtk.Switch switch_drive1_run;
	[GtkChild]
	private unowned Gtk.Switch switch_drive2_run;
	[GtkChild]
	private unowned Gtk.Switch switch_emergency;

	[GtkChild]
	private unowned Gtk.Button button_clear_errors;
	[GtkChild]
	private unowned Gtk.Button button_reset_device;

	[GtkChild]
	private unowned SpinButtonScale slider_speed1;
	[GtkChild]
	private unowned SpinButtonScale slider_torque1;
	[GtkChild]
	private unowned SpinButtonScale slider_speed2;
	[GtkChild]
	private unowned SpinButtonScale slider_torque2;

	public ControlPanel() {}

	construct
	{
		switch_power.notify["state"].connect((s, p) => {
			crd600_set_power_enabled(switch_power.state);
		});

		switch_drive1_run.notify["state"].connect((s, p) => {
			crd600_set_drive1_run_enabled(switch_drive1_run.state);
		});

		switch_drive2_run.notify["state"].connect((s, p) => {
			crd600_set_drive2_run_enabled(switch_drive2_run.state);
		});

		switch_emergency.notify["state"].connect((s, p) => {
			crd600_set_emergency_enabled(switch_emergency.state);
		});

		button_clear_errors.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "clear_errors");
		});

		button_reset_device.clicked.connect(() => {
			Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
					"Warning!",
					"Device will be reset.");
			dialog.add_response("cancel", "Cancel");
			dialog.add_response("continue", "Continue");
			dialog.set_response_appearance("cancel", DESTRUCTIVE);
			dialog.set_response_appearance("continue", SUGGESTED);
			dialog.response["continue"].connect(() => {
				ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "reset_device");
			});
			dialog.present();
		});

		slider_speed1.adjustment->value_changed.connect(() => {
			crd600_set_drive1_speed_ref(slider_speed1.value);
		});
		
		slider_torque1.adjustment->value_changed.connect(() => {
			crd600_set_drive1_torque_ref(slider_torque1.value / 100.0);
		});

		slider_speed2.adjustment->value_changed.connect(() => {
			crd600_set_drive2_speed_ref(slider_speed2.value);
		});
		
		slider_torque2.adjustment->value_changed.connect(() => {
			crd600_set_drive2_torque_ref(slider_torque2.value / 100.0);
		});
	}
}
	
	
}
	
	
	