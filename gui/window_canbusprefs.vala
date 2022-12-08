///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_canbusprefs.ui")]
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

	[GtkChild]
	private unowned Adw.ComboRow comborowWatchPeriod;

	[GtkChild]
	private unowned Gtk.StringList listWatchPeriod;

	private static bool _switchTpdoState = true;
	private static bool _switchWatchState = true;
	private static uint _watchPeriodSelected = 3;
	public const int watchPeriodDefault = 10;
	
	public WindowCanBusPrefs() {}

	construct
	{
		switchTpdo.set_active(_switchTpdoState);
		switchWatch.set_active(_switchWatchState);
		comborowWatchPeriod.set_selected(_watchPeriodSelected);

		buttonConnect.clicked.connect(() => {
			cansocket_connect(listCanInterface.get_string(comborowCanInterface.selected)
					, int.parse(listCanBitrate.get_string(comborowCanBitrate.selected)));
		});
		buttonDisconnect.clicked.connect(cansocket_disconnect);

		switchTpdo.notify["state"].connect((s,p) => {
			ucanopen_client_set_tpdo_enabled(switchTpdo.state);
			_switchTpdoState = switchTpdo.state;
		});

		switchWatch.notify["state"].connect((s,p) => {
			ucanopen_client_set_watch_enabled(switchWatch.state);
			_switchWatchState = switchWatch.state;
		});

		comborowWatchPeriod.notify["selected"].connect((s,p) => {
			ucanopen_client_set_watch_period(int.parse(listWatchPeriod.get_string(comborowWatchPeriod.selected)));
			_watchPeriodSelected = comborowWatchPeriod.selected;
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

    
