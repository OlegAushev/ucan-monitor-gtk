///
///
///


extern void motordrive_observer_get_watch_value(string name, string value);


[GtkTemplate (ui = "/src/gui/data_tables.ui")]
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



	public DataTables()
	{

		
	}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		updateSystemData();
		updateMotorData();
		updateInverterData();
		return true;
	}

	public void updateSystemData()
	{
		string result = "";
		motordrive_observer_get_watch_value("UPTIME", result);
		entryUptime.entry_text = result;

		motordrive_observer_get_watch_value("DRIVE_STATE", result);
		entryState.entry_text = result;

		motordrive_observer_get_watch_value("FAULTS", result);
		entryErrors.entry_text = result;

		motordrive_observer_get_watch_value("WARNINGS", result);
		entryWarnings.entry_text = result;
	}

	public void updateMotorData()
	{
		string result = "";
		motordrive_observer_get_watch_value("SPEED_RPM", result);
		entrySpeed.entry_text = result;

		motordrive_observer_get_watch_value("TORQUE", result);
		entryTorque.entry_text = result;

		motordrive_observer_get_watch_value("STATOR_CURRENT", result);
		entryCurrS.entry_text = result;

		motordrive_observer_get_watch_value("FIELD_CURRENT", result);
		entryCurrF.entry_text = result;

		motordrive_observer_get_watch_value("D_CURRENT", result);
		entryCurrD.entry_text = result;

		motordrive_observer_get_watch_value("Q_CURRENT", result);
		entryCurrQ.entry_text = result;

		motordrive_observer_get_watch_value("GAMMA_ANGLE_DEG", result);
		entryGamma.entry_text = result;

		motordrive_observer_get_watch_value("MOTOR_S_TEMP", result);
		entryTempS.entry_text = result;

		motordrive_observer_get_watch_value("MOTOR_FW_TEMP", result);
		entryTempFW.entry_text = result;

		motordrive_observer_get_watch_value("MECH_POWER", result);
		entryPowerMech.entry_text = result;
	}

	public void updateInverterData()
	{
		string result = "";
		motordrive_observer_get_watch_value("DC_VOLTAGE", result);
		entryVoltDC.entry_text = result;

		motordrive_observer_get_watch_value("DC_CURRENT", result);
		entryCurrDC.entry_text = result;

		motordrive_observer_get_watch_value("PHA_CURRENT", result);
		entryCurrPhA.entry_text = result;

		motordrive_observer_get_watch_value("PHB_CURRENT", result);
		entryCurrPhB.entry_text = result;

		motordrive_observer_get_watch_value("PHC_CURRENT", result);
		entryCurrPhC.entry_text = result;

		motordrive_observer_get_watch_value("PHA_TEMP", result);
		entryTempPhA.entry_text = result;

		motordrive_observer_get_watch_value("PHB_TEMP", result);
		entryTempPhB.entry_text = result;

		motordrive_observer_get_watch_value("PHC_TEMP", result);
		entryTempPhC.entry_text = result;

		motordrive_observer_get_watch_value("CASE_TEMP", result);
		entryTempAir.entry_text = result;

		motordrive_observer_get_watch_value("OUT_ELEC_POWER", result);
		entryPowerElec.entry_text = result;
	}
}


