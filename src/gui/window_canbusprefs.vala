///
///
///


extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();
extern void ucanopen_client_set_tpdo_state(bool state);


namespace CanMonitor {


[GtkTemplate (ui = "/src/gui/window_canbusprefs.ui")]
public class WindowCanBusPrefs : Adw.PreferencesWindow
{
	[GtkChild]
	private unowned Gtk.Button buttonConnect; 

	[GtkChild]
	private unowned Gtk.Button buttonDisconnect;

	[GtkChild]
	private unowned Adw.ComboRow comborowCanInterface;

	[GtkChild]
	private unowned Adw.ComboRow comborowCanBitrate;

	[GtkChild]
	private unowned Gtk.StringList listCanInterface;

	[GtkChild]
	private unowned Gtk.StringList listCanBitrate;

	[GtkChild]
	private unowned Gtk.Switch switchTpdo;

	private static bool _tpdoState = false;
	
	public WindowCanBusPrefs()
	{

	}

	construct
	{
		switchTpdo.set_active(_tpdoState);

		buttonConnect.clicked.connect(() => {
				cansocket_connect(listCanInterface.get_string(comborowCanInterface.get_selected())
				, int.parse(listCanBitrate.get_string(comborowCanBitrate.get_selected())));
		});
		buttonDisconnect.clicked.connect(cansocket_disconnect);

		switchTpdo.notify["state"].connect((s,p) => {
			ucanopen_client_set_tpdo_state(switchTpdo.state);
			_tpdoState = switchTpdo.state;
		});
	}
}


}

    
