///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_canbuspreferences.ui")]
public class WindowCanBusPreferences : Adw.PreferencesWindow
{
	[GtkChild]
	private unowned Adw.ComboRow comborow_can_interface;
	[GtkChild]
	private unowned Gtk.StringList list_can_interface;

	[GtkChild]
	private unowned Adw.ComboRow comborow_can_bitrate;
	[GtkChild]
	private unowned Gtk.StringList list_can_bitrate;
	
	[GtkChild]
	private unowned Gtk.Button button_connect;
	[GtkChild]
	private unowned Gtk.Button button_disconnect;

	[GtkChild]
	private unowned Gtk.Adjustment adjustment_client_id;
	[GtkChild]
	private unowned Gtk.Adjustment adjustment_server_id;

	[GtkChild]
	private unowned Gtk.Switch switch_tpdo;

	[GtkChild]
	private unowned Adw.ExpanderRow expanderrow_sync;
	[GtkChild]
	private unowned Gtk.Adjustment adjustment_sync_period;

	[GtkChild]
	private unowned Adw.ExpanderRow expanderrow_watch;
	[GtkChild]
	private unowned Gtk.Adjustment adjustment_watch_period;

	private static uint _client_id = 2;
	private static uint _server_id = 1;

	private static bool _tpdo_state = true;
	private static bool _sync_state = true;
	private static int _sync_period = 200;
	private static bool _watch_state = true;
	private static int _watch_period = 10;
	
	public WindowCanBusPreferences() {}

	construct
	{
		adjustment_client_id.value = _client_id;
		adjustment_server_id.value = _server_id;

		switch_tpdo.active = _tpdo_state;

		expanderrow_sync.enable_expansion = _sync_state;
		adjustment_sync_period.value = _sync_period;

		expanderrow_watch.enable_expansion = _watch_state;
		adjustment_watch_period.value = _watch_period;

		// SIGNALS
		button_connect.clicked.connect(() => {
			cansocket_connect(list_can_interface.get_string(comborow_can_interface.selected)
					, int.parse(list_can_bitrate.get_string(comborow_can_bitrate.selected)));
		});
		button_disconnect.clicked.connect(cansocket_disconnect);

		adjustment_client_id.value_changed.connect(() => {
			if (adjustment_client_id.value == adjustment_server_id.value)
			{
				adjustment_client_id.value = _client_id;
			}
			else
			{
				ucanopen_client_set_node_id((uint)adjustment_client_id.value);
				_client_id = (uint)adjustment_client_id.value;
			}
		});

		adjustment_server_id.value_changed.connect(() => {
			if (adjustment_server_id.value == adjustment_client_id.value)
			{
				adjustment_server_id.value = _server_id;
			}
			else
			{
				ucanopen_client_set_server_id(Backend.Ucanopen.server, (uint)adjustment_server_id.value);
				_server_id = (uint)adjustment_server_id.value;
			}
		});

		switch_tpdo.notify["state"].connect((s,p) => {
			ucanopen_client_set_tpdo_enabled(switch_tpdo.state);
			_tpdo_state = switch_tpdo.state;
		});

		expanderrow_sync.notify["enable-expansion"].connect((s,p) => {
			ucanopen_client_set_sync_enabled(expanderrow_sync.enable_expansion);
			_sync_state = expanderrow_sync.enable_expansion;
		});

		adjustment_sync_period.value_changed.connect(() => {
			ucanopen_client_set_sync_period((int)adjustment_sync_period.value);
			_sync_period = (int)adjustment_sync_period.value;
		});

		expanderrow_watch.notify["enable-expansion"].connect((s,p) => {
			ucanopen_client_set_watch_enabled(expanderrow_watch.enable_expansion);
			_watch_state = expanderrow_watch.enable_expansion;
		});

		adjustment_watch_period.value_changed.connect(() => {
			ucanopen_client_set_watch_period((int)adjustment_watch_period.value);
			_watch_period = (int)adjustment_watch_period.value;
		});
	}

	public static uint client_id
	{
		get { return _client_id; }
	}

	public static uint server_id
	{
		get { return _server_id; }
	}

	public static bool tpdo_state
	{
		get { return _tpdo_state; }
	}

	public static bool sync_state
	{
		get { return _sync_state; }
	}

	public static int sync_period
	{
		get { return _sync_period; }
	}

	public static bool watch_state
	{
		get { return _watch_state; }
	}

	public static int watch_period
	{
		get { return _watch_period; }
	}
}


}

    
