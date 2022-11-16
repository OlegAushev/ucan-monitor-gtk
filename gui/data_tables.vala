///
///
///


[GtkTemplate (ui = "/gui/data_tables.ui")]
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
	private unowned TableEntry tpdo1Indicator;
	[GtkChild]
	private unowned TableEntry tpdo2Indicator;
	[GtkChild]
	private unowned TableEntry tpdo3Indicator;
	[GtkChild]
	private unowned TableEntry tpdo4Indicator;



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
		updateTpdoStatus();
		return true;
	}

	public void updateSystemData()
	{
		string result = "";
		srmdrive_observer_get_watch_value("UPTIME", result);
		entryUptime.entry_text = result;

		srmdrive_observer_get_watch_value("DRIVE_STATE", result);
		entryState.entry_text = result;

		srmdrive_observer_get_watch_value("FAULTS", result);
		entryErrors.entry_text = result;

		srmdrive_observer_get_watch_value("WARNINGS", result);
		entryWarnings.entry_text = result;
	}

	public void updateMotorData()
	{
		string result = "";
		srmdrive_observer_get_watch_value("SPEED_RPM", result);
		entrySpeed.entry_text = result;

		srmdrive_observer_get_watch_value("TORQUE", result);
		entryTorque.entry_text = result;

		srmdrive_observer_get_watch_value("STATOR_CURRENT", result);
		entryCurrS.entry_text = result;

		srmdrive_observer_get_watch_value("FIELD_CURRENT", result);
		entryCurrF.entry_text = result;

		srmdrive_observer_get_watch_value("D_CURRENT", result);
		entryCurrD.entry_text = result;

		srmdrive_observer_get_watch_value("Q_CURRENT", result);
		entryCurrQ.entry_text = result;

		srmdrive_observer_get_watch_value("GAMMA_ANGLE_DEG", result);
		entryGamma.entry_text = result;

		srmdrive_observer_get_watch_value("MOTOR_S_TEMP", result);
		entryTempS.entry_text = result;

		srmdrive_observer_get_watch_value("MOTOR_FW_TEMP", result);
		entryTempFW.entry_text = result;

		srmdrive_observer_get_watch_value("MECH_POWER", result);
		entryPowerMech.entry_text = result;
	}

	public void updateInverterData()
	{
		string result = "";
		srmdrive_observer_get_watch_value("DC_VOLTAGE", result);
		entryVoltDC.entry_text = result;

		srmdrive_observer_get_watch_value("DC_CURRENT", result);
		entryCurrDC.entry_text = result;

		srmdrive_observer_get_watch_value("PHA_CURRENT", result);
		entryCurrPhA.entry_text = result;

		srmdrive_observer_get_watch_value("PHB_CURRENT", result);
		entryCurrPhB.entry_text = result;

		srmdrive_observer_get_watch_value("PHC_CURRENT", result);
		entryCurrPhC.entry_text = result;

		srmdrive_observer_get_watch_value("PHA_TEMP", result);
		entryTempPhA.entry_text = result;

		srmdrive_observer_get_watch_value("PHB_TEMP", result);
		entryTempPhB.entry_text = result;

		srmdrive_observer_get_watch_value("PHC_TEMP", result);
		entryTempPhC.entry_text = result;

		srmdrive_observer_get_watch_value("CASE_TEMP", result);
		entryTempAir.entry_text = result;

		srmdrive_observer_get_watch_value("OUT_ELEC_POWER", result);
		entryPowerElec.entry_text = result;
	}

	public void updateTpdoStatus()
	{
		if (srmdrive_is_tpdo_ok(0))
		{
			tpdo1Indicator.entry_text = "ok";
			tpdo1Indicator.remove_css_class("error");
			tpdo1Indicator.add_css_class("success");
		}
		else
		{
			tpdo1Indicator.entry_text = "bad";
			tpdo1Indicator.remove_css_class("success");
			tpdo1Indicator.add_css_class("error");
		}

		if (srmdrive_is_tpdo_ok(1))
		{
			tpdo2Indicator.entry_text = "ok";
			tpdo2Indicator.remove_css_class("error");
			tpdo2Indicator.add_css_class("success");
		}
		else
		{
			tpdo2Indicator.entry_text = "bad";
			tpdo2Indicator.remove_css_class("success");
			tpdo2Indicator.add_css_class("error");
		}

		if (srmdrive_is_tpdo_ok(2))
		{
			tpdo3Indicator.entry_text = "ok";
			tpdo3Indicator.remove_css_class("error");
			tpdo3Indicator.add_css_class("success");
		}
		else
		{
			tpdo3Indicator.entry_text = "bad";
			tpdo3Indicator.remove_css_class("success");
			tpdo3Indicator.add_css_class("error");
		}

		if (srmdrive_is_tpdo_ok(3))
		{
			tpdo4Indicator.entry_text = "ok";
			tpdo4Indicator.remove_css_class("error");
			tpdo4Indicator.add_css_class("success");
		}
		else
		{
			tpdo4Indicator.entry_text = "bad";
			tpdo4Indicator.remove_css_class("success");
			tpdo4Indicator.add_css_class("error");
		}
	}
}


