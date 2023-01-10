///
///
///


namespace LaunchPad {


[GtkTemplate (ui = "/gui/launchpad/launchpad_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
	[GtkChild]
	private unowned TableEntry entry_uptime;

	[GtkChild]
	private unowned TableBoolEntry tpdo1_indicator;
	
	[GtkChild]
	private unowned TableBoolEntry tpdo2_indicator;
	
	[GtkChild]
	private unowned TableBoolEntry tpdo3_indicator;
	
	[GtkChild]
	private unowned TableBoolEntry tpdo4_indicator;


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
		entry_uptime.entry_text = buf;
	}

	private void _update_tpdo1_data()
	{
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
	}

	private void _update_tpdo2_data()
	{
		tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
	}

	private void _update_tpdo3_data()
	{
		tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
	}

	private void _update_tpdo4_data()
	{
		tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
	}
}


}


