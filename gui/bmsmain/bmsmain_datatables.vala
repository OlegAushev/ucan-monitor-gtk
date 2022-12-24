///
///
///


namespace BmsMain {


[GtkTemplate (ui = "/gui/bmsmain/bmsmain_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entry_current;
	[GtkChild]
	private unowned TableEntry entry_temp_min;
	[GtkChild]
	private unowned TableEntry entry_temp_max;
	[GtkChild]
	private unowned TableEntry entry_charge;
	[GtkChild]
	private unowned TableEntry entry_voltage;

	[GtkChild]
	private unowned TableEntry heartbeat_indicator;
	[GtkChild]
	private unowned TableCheckEntry tpdo1_indicator;


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
		entry_voltage.entry_text = bmsmain_tpdo1_get_voltage().to_string();
		entry_current.entry_text = bmsmain_tpdo1_get_current().to_string();
		entry_temp_min.entry_text = bmsmain_tpdo1_get_temp_min().to_string();
		entry_temp_max.entry_text = bmsmain_tpdo1_get_temp_max().to_string();
		entry_charge.entry_text = bmsmain_tpdo1_get_charge().to_string();

		//string entryText = string.nfill(16, '\0');
		/*ucanopen_server_get_watch_value(Backend.Ucanopen.server, "UPTIME", entryText, 16);
		entryUptime.entry_text = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DRIVE_STATE", entryText, 16);
		entryState.entry_text = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "FAULTS", entryText, 16);
		entryErrors.entry_text = entryText;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "WARNINGS", entryText, 16);
		entryWarnings.entry_text = entryText;*/
	}

	public void update_connection_status()
	{
		if (!ucanopen_server_is_heartbeat_ok(Backend.Ucanopen.server))
		{
			heartbeat_indicator.entry_remove_css_class("success");
			heartbeat_indicator.entry_add_css_class("error");
			heartbeat_indicator.entry_text = "no HB";
		}
		else
		{
			heartbeat_indicator.entry_remove_css_class("error");
			heartbeat_indicator.entry_add_css_class("success");
			string nmtState = string.nfill(16, '\0');
			ucanopen_server_get_nmt_state(Backend.Ucanopen.server, nmtState, 16);
			heartbeat_indicator.entry_text = nmtState;
		}

		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
	}
}


}


