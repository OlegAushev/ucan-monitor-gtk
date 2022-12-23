///
///
///


namespace BmsMain {


[GtkTemplate (ui = "/gui/bmsmain/bmsmain_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entryCurrent;
	[GtkChild]
	private unowned TableEntry entryTempMin;
	[GtkChild]
	private unowned TableEntry entryTempMax;
	[GtkChild]
	private unowned TableEntry entryCharge;
	[GtkChild]
	private unowned TableEntry entryVoltage;

	[GtkChild]
	private unowned TableEntry heartbeatIndicator;
	[GtkChild]
	private unowned TableBoolEntry tpdo1Indicator;


	public DataTables() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		updateTpdo1Data();
		updateConnectionStatus();
		return true;
	}

	public void updateTpdo1Data()
	{
		entryVoltage.entry_text = bmsmain_tpdo1_get_voltage().to_string();
		entryCurrent.entry_text = bmsmain_tpdo1_get_current().to_string();
		entryTempMin.entry_text = bmsmain_tpdo1_get_temp_min().to_string();
		entryTempMax.entry_text = bmsmain_tpdo1_get_temp_max().to_string();
		entryCharge.entry_text = bmsmain_tpdo1_get_charge().to_string();

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

	public void updateConnectionStatus()
	{
		if (!ucanopen_server_is_heartbeat_ok(Backend.Ucanopen.server))
		{
			heartbeatIndicator.entry_remove_css_class("success");
			heartbeatIndicator.entry_add_css_class("error");
			heartbeatIndicator.entry_text = "no HB";
		}
		else
		{
			heartbeatIndicator.entry_remove_css_class("error");
			heartbeatIndicator.entry_add_css_class("success");
			string nmtState = string.nfill(16, '\0');
			ucanopen_server_get_nmt_state(Backend.Ucanopen.server, nmtState, 16);
			heartbeatIndicator.entry_text = nmtState;
		}

		tpdo1Indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
	}
}


}


