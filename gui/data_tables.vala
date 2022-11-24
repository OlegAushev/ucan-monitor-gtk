///
///
///


[GtkTemplate (ui = "/gui/data_tables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entryVoltageDC;
	[GtkChild]
	private unowned TableEntry entryTorque;
	[GtkChild]
	private unowned TableEntry entryTempMotor;
	[GtkChild]
	private unowned TableEntry entryTempBoard;
	[GtkChild]
	private unowned TableEntry entryTempIgbt;
	[GtkChild]
	private unowned TableEntry entryTempDriver;

	[GtkChild]
	private unowned TableBoolEntry entryResponseConnection;
	[GtkChild]
	private unowned TableBoolEntry entryTemperaturesConnection;




	//  [GtkChild]
	//  private unowned TableEntry entryTorque;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrS;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrF;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrD;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrQ;
	//  [GtkChild]
	//  private unowned TableEntry entryGamma;
	//  [GtkChild]
	//  private unowned TableEntry entryTempS;
	//  [GtkChild]
	//  private unowned TableEntry entryTempFW;
	//  [GtkChild]
	//  private unowned TableEntry entryPowerMech;

	//  [GtkChild]
	//  private unowned TableEntry entryVoltDC;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrDC;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrPhA;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrPhB;
	//  [GtkChild]
	//  private unowned TableEntry entryCurrPhC;
	//  [GtkChild]
	//  private unowned TableEntry entryTempPhA;
	//  [GtkChild]
	//  private unowned TableEntry entryTempPhB;
	//  [GtkChild]
	//  private unowned TableEntry entryTempPhC;
	//  [GtkChild]
	//  private unowned TableEntry entryTempAir;
	//  [GtkChild]
	//  private unowned TableEntry entryPowerElec;

	//  [GtkChild]
	//  private unowned TableBoolEntry tpdo1Indicator;
	//  [GtkChild]
	//  private unowned TableBoolEntry tpdo2Indicator;
	//  [GtkChild]
	//  private unowned TableBoolEntry tpdo3Indicator;
	//  [GtkChild]
	//  private unowned TableBoolEntry tpdo4Indicator;


	public DataTables() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		updateConnectionStatus();
		updateLeafInverterData();
		return true;
	}

	public void updateConnectionStatus()
	{
		entryResponseConnection.value = atv_leaf_inverter_is_tx_ok(0x1DA);
		entryTemperaturesConnection.value = atv_leaf_inverter_is_tx_ok(0x55A);
	}

	public void updateLeafInverterData()
	{
		char[] buf = new char[double.DTOSTR_BUF_SIZE];
		entryVoltageDC.entry_text = atv_leaf_inverter_get_voltagedc().format(buf, "%.2f");
		entryTorque.entry_text = atv_leaf_inverter_get_torque().format(buf, "%.2f");
		entryTempBoard.entry_text = atv_leaf_inverter_get_temp_board().format(buf, "%.2f");
		entryTempIgbt.entry_text = atv_leaf_inverter_get_temp_igbt().format(buf, "%.2f");
		entryTempDriver.entry_text = atv_leaf_inverter_get_temp_igbt_driver().format(buf, "%.2f");
		entryTempMotor.entry_text = atv_leaf_inverter_get_temp_motor().format(buf, "%.2f");










		//string result = "";
		//  srmdrive_observer_get_watch_value("UPTIME", result);
		//  entryUptime.entry_text = result;

		//  srmdrive_observer_get_watch_value("DRIVE_STATE", result);
		//  entryState.entry_text = result;

		//  srmdrive_observer_get_watch_value("FAULTS", result);
		//  entryErrors.entry_text = result;

		//  srmdrive_observer_get_watch_value("WARNINGS", result);
		//  entryWarnings.entry_text = result;
	}

	public void updateMotorData()
	{
		string result = "";
		//  srmdrive_observer_get_watch_value("SPEED_RPM", result);
		//  entrySpeed.entry_text = result;

		//  srmdrive_observer_get_watch_value("TORQUE", result);
		//  entryTorque.entry_text = result;

		//  srmdrive_observer_get_watch_value("STATOR_CURRENT", result);
		//  entryCurrS.entry_text = result;

		//  srmdrive_observer_get_watch_value("FIELD_CURRENT", result);
		//  entryCurrF.entry_text = result;

		//  srmdrive_observer_get_watch_value("D_CURRENT", result);
		//  entryCurrD.entry_text = result;

		//  srmdrive_observer_get_watch_value("Q_CURRENT", result);
		//  entryCurrQ.entry_text = result;

		//  srmdrive_observer_get_watch_value("GAMMA_ANGLE_DEG", result);
		//  entryGamma.entry_text = result;

		//  srmdrive_observer_get_watch_value("MOTOR_S_TEMP", result);
		//  entryTempS.entry_text = result;

		//  srmdrive_observer_get_watch_value("MOTOR_FW_TEMP", result);
		//  entryTempFW.entry_text = result;

		//  srmdrive_observer_get_watch_value("MECH_POWER", result);
		//  entryPowerMech.entry_text = result;
	}

	public void updateInverterData()
	{
		string result = "";
		//  srmdrive_observer_get_watch_value("DC_VOLTAGE", result);
		//  entryVoltDC.entry_text = result;

		//  srmdrive_observer_get_watch_value("DC_CURRENT", result);
		//  entryCurrDC.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHA_CURRENT", result);
		//  entryCurrPhA.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHB_CURRENT", result);
		//  entryCurrPhB.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHC_CURRENT", result);
		//  entryCurrPhC.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHA_TEMP", result);
		//  entryTempPhA.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHB_TEMP", result);
		//  entryTempPhB.entry_text = result;

		//  srmdrive_observer_get_watch_value("PHC_TEMP", result);
		//  entryTempPhC.entry_text = result;

		//  srmdrive_observer_get_watch_value("CASE_TEMP", result);
		//  entryTempAir.entry_text = result;

		//  srmdrive_observer_get_watch_value("OUT_ELEC_POWER", result);
		//  entryPowerElec.entry_text = result;
	}
}


