///
///
///


namespace Srmdrive {


[GtkTemplate (ui = "/gui/srmdrive_datatables.ui")]
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
		srmdrive_observer_get_watch_value("UPTIME", entryText, 16);
		entryUptime.entry_text = entryText;

		srmdrive_observer_get_watch_value("DRIVE_STATE", entryText, 16);
		entryState.entry_text = entryText;

		srmdrive_observer_get_watch_value("FAULTS", entryText, 16);
		entryErrors.entry_text = entryText;

		srmdrive_observer_get_watch_value("WARNINGS", entryText, 16);
		entryWarnings.entry_text = entryText;
	}

	public void updateMotorData()
	{
		string entryText = string.nfill(16, '\0');
		srmdrive_observer_get_watch_value("SPEED_RPM", entryText, 16);
		entrySpeed.entry_text = entryText;

		srmdrive_observer_get_watch_value("TORQUE", entryText, 16);
		entryTorque.entry_text = entryText;

		srmdrive_observer_get_watch_value("STATOR_CURRENT", entryText, 16);
		entryCurrS.entry_text = entryText;

		srmdrive_observer_get_watch_value("FIELD_CURRENT", entryText, 16);
		entryCurrF.entry_text = entryText;

		srmdrive_observer_get_watch_value("D_CURRENT", entryText, 16);
		entryCurrD.entry_text = entryText;

		srmdrive_observer_get_watch_value("Q_CURRENT", entryText, 16);
		entryCurrQ.entry_text = entryText;

		srmdrive_observer_get_watch_value("GAMMA_ANGLE_DEG", entryText, 16);
		entryGamma.entry_text = entryText;

		srmdrive_observer_get_watch_value("MOTOR_S_TEMP", entryText, 16);
		entryTempS.entry_text = entryText;

		srmdrive_observer_get_watch_value("MOTOR_FW_TEMP", entryText, 16);
		entryTempFW.entry_text = entryText;

		srmdrive_observer_get_watch_value("MECH_POWER", entryText, 16);
		entryPowerMech.entry_text = entryText;
	}

	public void updateInverterData()
	{
		string entryText = string.nfill(16, '\0');
		srmdrive_observer_get_watch_value("DC_VOLTAGE", entryText, 16);
		entryVoltDC.entry_text = entryText;

		srmdrive_observer_get_watch_value("DC_CURRENT", entryText, 16);
		entryCurrDC.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHA_CURRENT", entryText, 16);
		entryCurrPhA.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHB_CURRENT", entryText, 16);
		entryCurrPhB.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHC_CURRENT", entryText, 16);
		entryCurrPhC.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHA_TEMP", entryText, 16);
		entryTempPhA.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHB_TEMP", entryText, 16);
		entryTempPhB.entry_text = entryText;

		srmdrive_observer_get_watch_value("PHC_TEMP", entryText, 16);
		entryTempPhC.entry_text = entryText;

		srmdrive_observer_get_watch_value("CASE_TEMP", entryText, 16);
		entryTempAir.entry_text = entryText;

		srmdrive_observer_get_watch_value("OUT_ELEC_POWER", entryText, 16);
		entryPowerElec.entry_text = entryText;
	}

	public void updateConnectionStatus()
	{
		if (!srmdrive_is_heartbeat_ok())
		{
			heartbeatIndicator.entry_remove_css_class("success");
			heartbeatIndicator.entry_add_css_class("error");
		}
		else
		{
			heartbeatIndicator.entry_remove_css_class("error");
			heartbeatIndicator.entry_add_css_class("success");
			string nmtState = "";
			srmdrive_get_nmt_state(nmtState);
			heartbeatIndicator.entry_text = nmtState;
		}

		tpdo1Indicator.value = srmdrive_is_tpdo_ok(0);
		tpdo2Indicator.value = srmdrive_is_tpdo_ok(1);
		tpdo3Indicator.value = srmdrive_is_tpdo_ok(2);
		tpdo4Indicator.value = srmdrive_is_tpdo_ok(3);
	}
}


}


