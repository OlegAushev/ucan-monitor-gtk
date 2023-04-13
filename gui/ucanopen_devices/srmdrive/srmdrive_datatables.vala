///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/ucanopen_devices/srmdrive/srmdrive_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned BasicEntry entry_uptime;
	[GtkChild]
	private unowned BasicEntry entry_state;
	[GtkChild]
	private unowned BasicEntry entry_errors;
	[GtkChild]
	private unowned BasicEntry entry_warnings;

	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
	[GtkChild]
	private unowned BoolEntry tpdo2_indicator;
	[GtkChild]
	private unowned BoolEntry tpdo3_indicator;
	[GtkChild]
	private unowned BoolEntry tpdo4_indicator;

	[GtkChild]
	private unowned BasicEntry entry_tpdo2_raw_data;
	[GtkChild]
	private unowned BasicEntry entry_tpdo3_raw_data;
	[GtkChild]
	private unowned BasicEntry entry_tpdo4_raw_data;
    
    //------------------------------------------------------------------------------------------------------------------
	[GtkChild]
	private unowned BoolEntry tpdo1_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo1_raw_data;
    [GtkChild]
	private unowned BasicEntry entry_drive_state;
    [GtkChild]
	private unowned BoolEntry entry_run_status;
    [GtkChild]
	private unowned BoolEntry entry_error_status;
    [GtkChild]
	private unowned BoolEntry entry_warning_status;
    [GtkChild]
	private unowned BoolEntry entry_overheat_status;
    [GtkChild]
	private unowned BasicEntry entry_drive_reference;
    [GtkChild]
	private unowned BasicEntry entry_control_loop_type;
    [GtkChild]
	private unowned BasicEntry entry_dc_voltage;
    [GtkChild]
	private unowned BasicEntry entry_torque;
    [GtkChild]
	private unowned BasicEntry entry_speed;
    //------------------------------------------------------------------------------------------------------------------
    const size_t _entry_buf_len = 17;
	string _entry_buf = string.nfill(_entry_buf_len, '\0');

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
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "UPTIME", buf, 16);
		entry_uptime.text_value = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "DRIVE_STATE", buf, 16);
		entry_state.text_value = buf;

		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "FAULTS", buf, 16);
		uint error_code = uint.parse(buf);
		if (error_code != 0)
		{
			entry_errors.add_css_class("error");
			entry_errors.text_value = error_code.to_string("%X");
		}
		else
		{
			entry_errors.remove_css_class("error");
			entry_errors.text_value = buf;
		}

		//ucanopen_server_get_watch_value(Backend.Ucanopen.server, "WARNINGS", buf, 16);
		uint warning_code = ucanopen_devices_get_warning_code(Backend.Ucanopen.server);
		if (warning_code != 0)
		{
			entry_warnings.add_css_class("warning");
			entry_warnings.text_value = warning_code.to_string("%X");
		}
		else
		{
			entry_warnings.remove_css_class("warning");
			entry_warnings.text_value = buf;
		}
	}

	

	

	private void _update_tpdo1_data() {
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
		entry_tpdo1_raw_data.text_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 0).to_string("%016lX");

        srmdrive_tpdo1_get_drive_state(_entry_buf, _entry_buf_len);
        entry_drive_state.text_value = _entry_buf;

        entry_run_status.value = srmdrive_tpdo1_get_run_status();
        entry_error_status.value = srmdrive_tpdo1_get_error_status();
        entry_warning_status.value = srmdrive_tpdo1_get_warning_status();
        entry_overheat_status.value = srmdrive_tpdo1_get_overheat_status();

        srmdrive_tpdo1_get_drive_reference(_entry_buf, _entry_buf_len);
        entry_drive_reference.text_value = _entry_buf;

        srmdrive_tpdo1_get_drive_loop_type(_entry_buf, _entry_buf_len);
        entry_control_loop_type.text_value = _entry_buf;

        srmdrive_tpdo1_get_dc_voltage(_entry_buf, _entry_buf_len);
        entry_dc_voltage.text_value = _entry_buf;

        srmdrive_tpdo1_get_torque(_entry_buf, _entry_buf_len);
        entry_torque.text_value = _entry_buf;

        srmdrive_tpdo1_get_speed(_entry_buf, _entry_buf_len);
        entry_speed.text_value = _entry_buf;
	}



















	private void _update_tpdo2_data()
	{
		tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
		entry_tpdo2_raw_data.text_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 1).to_string("%016lX");
	}

	private void _update_tpdo3_data()
	{
		tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
		entry_tpdo3_raw_data.text_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 2).to_string("%016lX");
	}

	private void _update_tpdo4_data()
	{
		tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
		entry_tpdo4_raw_data.text_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 3).to_string("%016lX");
	}
}


}


