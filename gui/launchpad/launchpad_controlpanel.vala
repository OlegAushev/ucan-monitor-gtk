///
///
///


namespace LaunchPad {


[GtkTemplate (ui = "/gui/launchpad/launchpad_controlpanel.ui")]
public class ControlPanel : Adw.Bin
{
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
			launchpad_set_client_value(0, slider_server_rpdo1.value);
		});
		slider_server_rpdo2.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(1, slider_server_rpdo2.value);
		});
		slider_server_rpdo3.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(2, slider_server_rpdo3.value);
		});
		slider_server_rpdo4.adjustment->value_changed.connect(() => {
			launchpad_set_client_value(3, slider_server_rpdo4.value);
		});
	}
}
	
	
}
	
	
	