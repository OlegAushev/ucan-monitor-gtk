///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_canbuspreferences.ui")]
public class WindowCanBusPreferences : Adw.PreferencesWindow
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
	private unowned Gtk.SpinButton spinbuttonClientId;

	[GtkChild]
	private unowned Gtk.Adjustment adjustmentClientId;

	[GtkChild]
	private unowned Gtk.SpinButton spinbuttonServerId;

	[GtkChild]
	private unowned Gtk.Adjustment adjustmentServerId;

	[GtkChild]
	private unowned Gtk.Switch switchTpdo;

	[GtkChild]
	private unowned Gtk.Switch switchWatch;

	[GtkChild]
	private unowned Adw.ComboRow comborowWatchPeriod;

	[GtkChild]
	private unowned Gtk.StringList listWatchPeriod;

	private static uint _clientId = 2;
	private static uint _serverId = 1;

	private static bool _switchTpdoState = true;
	private static bool _switchWatchState = true;
	private static uint _watchPeriodSelected = 3;
	public const int watchPeriodDefault = 10;
	
	public WindowCanBusPreferences() {}

	construct
	{
		adjustmentClientId.value = _clientId;
		adjustmentServerId.value = _serverId;

		switchTpdo.active = _switchTpdoState;
		switchWatch.active = _switchWatchState;
		comborowWatchPeriod.selected = _watchPeriodSelected;

		buttonConnect.clicked.connect(() => {
			cansocket_connect(listCanInterface.get_string(comborowCanInterface.selected)
					, int.parse(listCanBitrate.get_string(comborowCanBitrate.selected)));
		});
		buttonDisconnect.clicked.connect(cansocket_disconnect);

		adjustmentClientId.value_changed.connect(() => {
			if (adjustmentClientId.value == adjustmentServerId.value)
			{
				adjustmentClientId.value = _clientId;
			}
			else
			{
				ucanopen_client_set_nodeid((uint)adjustmentClientId.value);
				_clientId = (uint)adjustmentClientId.value;
			}
		});

		adjustmentServerId.value_changed.connect(() => {
			if (adjustmentServerId.value == adjustmentClientId.value)
			{
				adjustmentServerId.value = _serverId;
			}
			else
			{
				ucanopen_client_set_serverid(backend.ucanopenServer, (uint)adjustmentServerId.value);
				_serverId = (uint)adjustmentServerId.value;
			}
		});

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

	public static uint clientId
	{
		get { return _clientId; }
	}

	public static uint serverId
	{
		get { return _serverId; }
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

    
