///
///
///


namespace LaunchPad {


[GtkTemplate (ui = "/gui/ucanopen_devices/launchpad/launchpad_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
	[GtkChild]
	private unowned BasicEntry entry_uptime;

	[GtkChild]
	private unowned BoolEntry tpdo1_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo1_raw_data;
	
	[GtkChild]
	private unowned BoolEntry tpdo2_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo2_raw_data;
	
	[GtkChild]
	private unowned BoolEntry tpdo3_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo3_raw_data;
	
	[GtkChild]
	private unowned BoolEntry tpdo4_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo4_raw_data;


	public DataTables() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		_update_system_data();
		_update_tpdo1_data();
		_update_tpdo2_data();
		_update_tpdo3_data();
		_update_tpdo4_data();
		return true;
	}

	private void _update_system_data()
	{
		heartbeat_indicator.update();

		string buf = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "uptime", buf, 16);
		entry_uptime.string_value = buf;
	}

	private void _update_tpdo1_data()
	{
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
		entry_tpdo1_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 0).to_string("%016lX");
	}

	private void _update_tpdo2_data()
	{
		tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
		entry_tpdo2_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 1).to_string("%016lX");
	}

	private void _update_tpdo3_data()
	{
		tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
		entry_tpdo3_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 2).to_string("%016lX");
	}

	private void _update_tpdo4_data()
	{
		tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
		entry_tpdo4_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 3).to_string("%016lX");
	}
}


}


