///
///
///


extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();
extern void ucanopen_client_set_tpdo_enabled(bool isEnabled);
extern void motordrive_observer_set_watch_enabled(bool isEnabled);


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

	[GtkChild]
	private unowned Gtk.Switch switchWatch;

	private static bool _switchTpdoState = true;
	private static bool _switchWatchState = true;
	
	public WindowCanBusPrefs()
	{

	}

	construct
	{
		switchTpdo.set_active(_switchTpdoState);
		switchWatch.set_active(_switchWatchState);

		buttonConnect.clicked.connect(() => {
				cansocket_connect(listCanInterface.get_string(comborowCanInterface.get_selected())
				, int.parse(listCanBitrate.get_string(comborowCanBitrate.get_selected())));
		});
		buttonDisconnect.clicked.connect(cansocket_disconnect);

		switchTpdo.notify["state"].connect((s,p) => {
			ucanopen_client_set_tpdo_enabled(switchTpdo.state);
			_switchTpdoState = switchTpdo.state;
		});

		switchWatch.notify["state"].connect((s,p) => {
			motordrive_observer_set_watch_enabled(switchWatch.state);
			_switchWatchState = switchWatch.state;
		});
	}

	public static bool switchTpdoState
	{
		get { return _switchTpdoState; }
	}

	public static bool switchWatchState
	{
		get { return _switchWatchState; }
	}
}


}

    
