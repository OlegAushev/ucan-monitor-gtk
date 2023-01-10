///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/srmdrive/srmdrive_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entry_uptime;
	[GtkChild]
	private unowned TableEntry entry_state;
	[GtkChild]
	private unowned TableEntry entry_errors;
	[GtkChild]
	private unowned TableEntry entry_warnings;

	[GtkChild]
	private unowned TableEntry entry_speed;
	[GtkChild]
	private unowned TableEntry entry_torque;
	[GtkChild]
	private unowned TableEntry entry_curr_s;
	[GtkChild]
	private unowned TableEntry entry_curr_f;
	[GtkChild]
	private unowned TableEntry entry_curr_d;
	[GtkChild]
	private unowned TableEntry entry_curr_q;
	[GtkChild]
	private unowned TableEntry entry_gamma;
	[GtkChild]
	private unowned TableEntry entry_temp_s;
	[GtkChild]
	private unowned TableEntry entry_temp_fw;
	[GtkChild]
	private unowned TableEntry entry_power_mech;

	[GtkChild]
	private unowned TableEntry entry_volt_dc;
	[GtkChild]
	private unowned TableEntry entry_curr_dc;
	[GtkChild]
	private unowned TableEntry entry_curr_pha;
	[GtkChild]
	private unowned TableEntry entry_curr_phb;
	[GtkChild]
	private unowned TableEntry entry_curr_phc;
	[GtkChild]
	private unowned TableEntry entry_temp_pha;
	[GtkChild]
	private unowned TableEntry entry_temp_phb;
	[GtkChild]
	private unowned TableEntry entry_temp_phc;
	[GtkChild]
	private unowned TableEntry entry_temp_air;
	[GtkChild]
	private unowned TableEntry entry_power_elec;

	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
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
		update_system_data();
		update_motor_data();
		update_inverter_data();
		update_connection_status();
		return true;
	}

	public void update_system_data()
	{
		string buf = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "UPTIME", buf, 16);
		entry_uptime.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DRIVE_STATE", buf, 16);
		entry_state.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "FAULTS", buf, 16);
		uint error_code = uint.parse(buf);
		if (error_code != 0)
		{
			entry_errors.add_css_class("error");
			entry_errors.entry_text = error_code.to_string("%X");
		}
		else
		{
			entry_errors.remove_css_class("error");
			entry_errors.entry_text = buf;
		}

		//ucanopen_server_get_watch_value(Backend.Ucanopen.server, "WARNINGS", buf, 16);
		uint warning_code = ucanopen_devices_get_warning_code(Backend.Ucanopen.server);
		if (warning_code != 0)
		{
			entry_warnings.add_css_class("warning");
			entry_warnings.entry_text = warning_code.to_string("%X");
		}
		else
		{
			entry_warnings.remove_css_class("warning");
			entry_warnings.entry_text = buf;
		}
	}

	public void update_motor_data()
	{
		string buf = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "SPEED_RPM", buf, 16);
		entry_speed.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "TORQUE", buf, 16);
		entry_torque.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "STATOR_CURRENT", buf, 16);
		entry_curr_s.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "FIELD_CURRENT", buf, 16);
		entry_curr_f.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "D_CURRENT", buf, 16);
		entry_curr_d.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "Q_CURRENT", buf, 16);
		entry_curr_q.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "GAMMA_ANGLE_DEG", buf, 16);
		entry_gamma.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "MOTOR_S_TEMP", buf, 16);
		entry_temp_s.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "MOTOR_FW_TEMP", buf, 16);
		entry_temp_fw.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "MECH_POWER", buf, 16);
		entry_power_mech.entry_text = buf;
	}

	public void update_inverter_data()
	{
		string buf = string.nfill(16, '\0');
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DC_VOLTAGE", buf, 16);
		entry_volt_dc.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DC_CURRENT", buf, 16);
		entry_curr_dc.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHA_CURRENT", buf, 16);
		entry_curr_pha.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHB_CURRENT", buf, 16);
		entry_curr_phb.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHC_CURRENT", buf, 16);
		entry_curr_phc.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHA_TEMP", buf, 16);
		entry_temp_pha.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHB_TEMP", buf, 16);
		entry_temp_phb.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "PHC_TEMP", buf, 16);
		entry_temp_phc.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "CASE_TEMP", buf, 16);
		entry_temp_air.entry_text = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "OUT_ELEC_POWER", buf, 16);
		entry_power_elec.entry_text = buf;
	}

	public void update_connection_status()
	{
		heartbeat_indicator.update();
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
		tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
		tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
		tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
	}
}


}


