///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/srmdrive/srmdrive_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entryUptime;
	[GtkChild]
	private unowned TableEntry entryState;
	[GtkChild]
	private unowned TableEntry entryErrors;
	[GtkChild]
	private unowned TableEntry entryWarnings;

	[GtkChild]
	private unowned TableEntry entrySpeed;
	[GtkChild]
	private unowned TableEntry entryTorque;
	[GtkChild]
	private unowned TableEntry entryCurrS;
	[GtkChild]
	private unowned TableEntry entryCurrF;
	[GtkChild]
	private unowned TableEntry entryCurrD;
	[GtkChild]
	private unowned TableEntry entryCurrQ;
	[GtkChild]
	private unowned TableEntry entryGamma;
	[GtkChild]
	private unowned TableEntry entryTempS;
	[GtkChild]
	private unowned TableEntry entryTempFW;
	[GtkChild]
	private unowned TableEntry entryPowerMech;

	[GtkChild]
	private unowned TableEntry entryVoltDC;
	[GtkChild]
	private unowned TableEntry entryCurrDC;
	[GtkChild]
	private unowned TableEntry entryCurrPhA;
	[GtkChild]
	private unowned TableEntry entryCurrPhB;
	[GtkChild]
	private unowned TableEntry entryCurrPhC;
	[GtkChild]
	private unowned TableEntry entryTempPhA;
	[GtkChild]
	private unowned TableEntry entryTempPhB;
	[GtkChild]
	private unowned TableEntry entryTempPhC;
	[GtkChild]
	private unowned TableEntry entryTempAir;
	[GtkChild]
	private unowned TableEntry entryPowerElec;

	[GtkChild]
	private unowned TableEntry heartbeatIndicator;
	[GtkChild]
	private unowned TableBoolEntry tpdo1Indicator;
	[GtkChild]
	private unowned TableBoolEntry tpdo2Indicator;
	[GtkChild]
	private unowned TableBoolEntry tpdo3Indicator;
	[GtkChild]
	private unowned TableBoolEntry tpdo4Indicator;


	public DataTables() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		updateSystemData();
		updateMotorData();
		updateInverterData();
		updateConnectionStatus();
		return true;
	}

	public void updateSystemData()
	{
		string entryText = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(backend.ucanopenServer, "UPTIME", entryText, 16);
		entryUptime.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "DRIVE_STATE", entryText, 16);
		entryState.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "FAULTS", entryText, 16);
		entryErrors.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "WARNINGS", entryText, 16);
		entryWarnings.entry_text = entryText;
	}

	public void updateMotorData()
	{
		string entryText = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(backend.ucanopenServer, "SPEED_RPM", entryText, 16);
		entrySpeed.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "TORQUE", entryText, 16);
		entryTorque.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "STATOR_CURRENT", entryText, 16);
		entryCurrS.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "FIELD_CURRENT", entryText, 16);
		entryCurrF.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "D_CURRENT", entryText, 16);
		entryCurrD.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "Q_CURRENT", entryText, 16);
		entryCurrQ.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "GAMMA_ANGLE_DEG", entryText, 16);
		entryGamma.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "MOTOR_S_TEMP", entryText, 16);
		entryTempS.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "MOTOR_FW_TEMP", entryText, 16);
		entryTempFW.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "MECH_POWER", entryText, 16);
		entryPowerMech.entry_text = entryText;
	}

	public void updateInverterData()
	{
		string entryText = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(backend.ucanopenServer, "DC_VOLTAGE", entryText, 16);
		entryVoltDC.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "DC_CURRENT", entryText, 16);
		entryCurrDC.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHA_CURRENT", entryText, 16);
		entryCurrPhA.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHB_CURRENT", entryText, 16);
		entryCurrPhB.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHC_CURRENT", entryText, 16);
		entryCurrPhC.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHA_TEMP", entryText, 16);
		entryTempPhA.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHB_TEMP", entryText, 16);
		entryTempPhB.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "PHC_TEMP", entryText, 16);
		entryTempPhC.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "CASE_TEMP", entryText, 16);
		entryTempAir.entry_text = entryText;

		ucanopen_server_get_watch_value(backend.ucanopenServer, "OUT_ELEC_POWER", entryText, 16);
		entryPowerElec.entry_text = entryText;
	}

	public void updateConnectionStatus()
	{
		if (!ucanopen_server_is_heartbeat_ok(backend.ucanopenServer))
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
			ucanopen_server_get_nmt_state(backend.ucanopenServer, nmtState, 16);
			heartbeatIndicator.entry_text = nmtState;
		}

		tpdo1Indicator.value = ucanopen_server_is_tpdo_ok(backend.ucanopenServer, 0);
		tpdo2Indicator.value = ucanopen_server_is_tpdo_ok(backend.ucanopenServer, 1);
		tpdo3Indicator.value = ucanopen_server_is_tpdo_ok(backend.ucanopenServer, 2);
		tpdo4Indicator.value = ucanopen_server_is_tpdo_ok(backend.ucanopenServer, 3);
	}
}


}


