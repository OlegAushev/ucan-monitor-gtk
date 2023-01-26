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
	private unowned Gtk.Button button_reset_errors;
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





		button_reset_errors.clicked.connect(() => {
			ucanopen_server_exec(Backend.Ucanopen.server, "system", "syslog", "reset_errors");
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
				ucanopen_server_exec(Backend.Ucanopen.server, "system", "device", "reset_device");
			});
			dialog.present();
		});

		/*slider_client_tpdo1.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(0, slider_client_tpdo1.value);
		});
		slider_client_tpdo2.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(1, slider_client_tpdo2.value);
		});
		slider_client_tpdo3.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(2, slider_client_tpdo3.value);
		});
		slider_client_tpdo4.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(3, slider_client_tpdo4.value);
		});

		slider_server_rpdo1.adjustment->value_changed.connect(() => {
			launchpad_set_server_value(0, slider_server_rpdo1.value);
		});
		slider_server_rpdo2.adjustment->value_changed.connect(() => {
			launchpad_set_server_value(1, slider_server_rpdo2.value);
		});
		slider_server_rpdo3.adjustment->value_changed.connect(() => {
			launchpad_set_server_value(2, slider_server_rpdo3.value);
		});
		slider_server_rpdo4.adjustment->value_changed.connect(() => {
			launchpad_set_server_value(3, slider_server_rpdo4.value);
		});*/
	}
}
	
	
}
	
	
	