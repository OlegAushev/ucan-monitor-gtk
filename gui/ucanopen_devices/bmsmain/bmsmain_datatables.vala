///
///
///


namespace BmsMain {


[GtkTemplate (ui = "/gui/ucanopen_devices/bmsmain/bmsmain_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned BasicEntry entry_current;
	[GtkChild]
	private unowned BasicEntry entry_temp_min;
	[GtkChild]
	private unowned BasicEntry entry_temp_max;
	[GtkChild]
	private unowned BasicEntry entry_charge;
	[GtkChild]
	private unowned BasicEntry entry_voltage;

	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
	[GtkChild]
	private unowned CheckEntry tpdo1_indicator;


	public DataTables() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		update_tpdo1_data();
		update_connection_status();
		return true;
	}

	public void update_tpdo1_data()
	{
		entry_voltage.text_value = bmsmain_tpdo1_get_voltage().to_string();
		entry_current.text_value = bmsmain_tpdo1_get_current().to_string();
		entry_temp_min.text_value = bmsmain_tpdo1_get_temp_min().to_string();
		entry_temp_max.text_value = bmsmain_tpdo1_get_temp_max().to_string();
		entry_charge.text_value = bmsmain_tpdo1_get_charge().to_string();

		//string entryText = string.nfill(16, '\0');
		/*ucanopen_server_get_watch_value(Backend.Ucanopen.server, "UPTIME", entryText, 16);
		entryUptime.text_value = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DRIVE_STATE", entryText, 16);
		entryState.text_value = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "FAULTS", entryText, 16);
		entryErrors.text_value = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "WARNINGS", entryText, 16);
		entryWarnings.text_value = entryText;*/
	}

	public void update_connection_status()
	{
		heartbeat_indicator.update();
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
	}
}


}


