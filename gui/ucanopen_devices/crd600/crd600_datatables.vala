///
///
///


namespace Crd600 {


[GtkTemplate (ui = "/gui/ucanopen_devices/crd600/crd600_datatables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
	[GtkChild]
	private unowned BasicEntry entry_uptime;
	////////////////////////////////////////////////////////////////////////////////
	[GtkChild]
	private unowned BoolEntry tpdo1_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo1_raw_data;
	[GtkChild]
	private unowned BasicEntry entry_drive1_state;
	[GtkChild]
	private unowned BasicEntry entry_drive1_ref;
	[GtkChild]
	private unowned BoolEntry entry_drive1_run;
	[GtkChild]
	private unowned BasicEntry entry_drive2_state;
	[GtkChild]
	private unowned BasicEntry entry_drive2_ref;
	[GtkChild]
	private unowned BoolEntry entry_drive2_run;
	[GtkChild]
	private unowned BoolEntry entry_error_status;
	[GtkChild]
	private unowned BoolEntry entry_warning_status;
	[GtkChild]
	private unowned BoolEntry entry_overheat_status;
	[GtkChild]
	private unowned BasicEntry entry_control_loop_type;
	////////////////////////////////////////////////////////////////////////////////
	[GtkChild]
	private unowned BoolEntry tpdo2_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo2_raw_data;
	////////////////////////////////////////////////////////////////////////////////
	[GtkChild]
	private unowned BoolEntry tpdo3_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo3_raw_data;
	////////////////////////////////////////////////////////////////////////////////
	[GtkChild]
	private unowned BoolEntry tpdo4_indicator;
	[GtkChild]
	private unowned BasicEntry entry_tpdo4_raw_data;
	////////////////////////////////////////////////////////////////////////////////
	private const int _entry_buf_len = 10;
	private string _entry_buf = string.nfill(_entry_buf_len, '\0');

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
		ucanopen_server_get_watch_value(Backend.Ucanopen.server, "uptime", buf, 16);
		entry_uptime.string_value = buf;
	}

	private void _update_tpdo1_data()
	{
		tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
		entry_tpdo1_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 0).to_string("%016lX");
		
		crd600_tpdo1_get_drive1_state(_entry_buf, _entry_buf_len);
		entry_drive1_state.string_value = _entry_buf;
		crd600_tpdo1_get_drive1_ref(_entry_buf, _entry_buf_len);
		entry_drive1_ref.string_value = _entry_buf;
		entry_drive1_run.value = crd600_tpdo1_get_drive1_run();

		crd600_tpdo1_get_drive2_state(_entry_buf, _entry_buf_len);
		entry_drive2_state.string_value = _entry_buf;
		crd600_tpdo1_get_drive2_ref(_entry_buf, _entry_buf_len);
		entry_drive2_ref.string_value = _entry_buf;
		entry_drive2_run.value = crd600_tpdo1_get_drive2_run();

		entry_error_status.value = crd600_tpdo1_get_error();
		entry_warning_status.value = crd600_tpdo1_get_warning();
		entry_overheat_status.value = crd600_tpdo1_get_overheat();

		crd600_tpdo1_get_control_loop_type(_entry_buf, _entry_buf_len);
		entry_control_loop_type.string_value = _entry_buf;





	}

	private void _update_tpdo2_data()
	{
		tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
		entry_tpdo2_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 1).to_string("%016lX");
	}

	private void _update_tpdo3_data()
	{
		tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
		entry_tpdo3_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 2).to_string("%016lX");
	}

	private void _update_tpdo4_data()
	{
		tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
		entry_tpdo4_raw_data.string_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 3).to_string("%016lX");
	}
}


}


