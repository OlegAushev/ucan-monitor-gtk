///
///
///


namespace LaunchPad {


[GtkTemplate (ui = "/gui/ucanopen_devices/launchpad/launchpad_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Button button_clear_errors;
	[GtkChild]
	private unowned Gtk.Button button_reset_device;

	[GtkChild]
	private unowned SpinButtonScale slider_client_tpdo1;
	[GtkChild]
	private unowned SpinButtonScale slider_client_tpdo2;
	[GtkChild]
	private unowned SpinButtonScale slider_client_tpdo3;
	[GtkChild]
	private unowned SpinButtonScale slider_client_tpdo4;
	[GtkChild]
	private unowned SpinButtonScale slider_server_rpdo1;
	[GtkChild]
	private unowned SpinButtonScale slider_server_rpdo2;
	[GtkChild]
	private unowned SpinButtonScale slider_server_rpdo3;
	[GtkChild]
	private unowned SpinButtonScale slider_server_rpdo4;

	public ControlPanel() {}

	construct
	{
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

		slider_client_tpdo1.adjustment->value_changed.connect(() => {
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
		});
	}
}
	
	
}
	
	
	