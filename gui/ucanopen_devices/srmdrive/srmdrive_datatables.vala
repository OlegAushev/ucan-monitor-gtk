///
///
///


namespace SrmDrive {


[GtkTemplate (ui = "/gui/ucanopen_devices/srmdrive/srmdrive_datatables.ui")]
public class DataTables : Adw.Bin
{
    [GtkChild]
    private unowned Ucanopen.HeartbeatIndicator heartbeat_indicator;
    [GtkChild]
    private unowned BasicEntry entry_uptime;
    
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
    [GtkChild]
    private unowned BoolEntry tpdo2_indicator;
    [GtkChild]
    private unowned BasicEntry entry_tpdo2_raw_data;
    [GtkChild]
    private unowned BasicEntry entry_stator_current;
    [GtkChild]
    private unowned BasicEntry entry_field_current;
    [GtkChild]
    private unowned BasicEntry entry_out_voltage;
    [GtkChild]
    private unowned BasicEntry entry_mech_power;
    [GtkChild]
    private unowned BasicEntry entry_elec_power;
   
    //------------------------------------------------------------------------------------------------------------------    
    [GtkChild]
    private unowned BoolEntry tpdo3_indicator;
    [GtkChild]
    private unowned BasicEntry entry_tpdo3_raw_data;
    [GtkChild]
    private unowned BasicEntry entry_pwrmodule_temp;
    [GtkChild]
    private unowned BasicEntry entry_excmodule_temp;
    [GtkChild]
    private unowned BasicEntry entry_pcb_temp;
    [GtkChild]
    private unowned BasicEntry entry_aw_temp;
    [GtkChild]
    private unowned BasicEntry entry_fw_temp;

    //------------------------------------------------------------------------------------------------------------------    
    [GtkChild]
    private unowned BoolEntry tpdo4_indicator;
    [GtkChild]
    private unowned BasicEntry entry_tpdo4_raw_data;
    [GtkChild]
    private unowned BasicEntry entry_errors;
    [GtkChild]
    private unowned BasicEntry entry_warnings;

    //------------------------------------------------------------------------------------------------------------------
    private const int _entry_buf_len = 17;
    private string _entry_buf = string.nfill(_entry_buf_len, '\0');

    public DataTables() {}

    construct
    {
        Timeout.add(50, update);
    }
    
    public bool update()
    {
        _update_watch_data();
        _update_tpdo1_data();
        _update_tpdo2_data();
        _update_tpdo3_data();
        _update_tpdo4_data();
        return true;
    }

    private void _update_watch_data()
    {
        heartbeat_indicator.update();

        string buf = string.nfill(16, '\0');
        ucanopen_server_get_watch_value(Backend.Ucanopen.server, "uptime", buf, 16);
        entry_uptime.string_value = buf;
    }


    private void _update_tpdo1_data() {
        tpdo1_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 0);
        entry_tpdo1_raw_data.ulong_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 0);

        srmdrive_tpdo1_get_drive_state(_entry_buf, _entry_buf_len);
        entry_drive_state.string_value = _entry_buf;

        entry_run_status.value = srmdrive_tpdo1_get_run_status();
        entry_error_status.value = srmdrive_tpdo1_get_error_status();
        entry_warning_status.value = srmdrive_tpdo1_get_warning_status();
        entry_overheat_status.value = srmdrive_tpdo1_get_overheat_status();

        srmdrive_tpdo1_get_drive_reference(_entry_buf, _entry_buf_len);
        entry_drive_reference.string_value = _entry_buf;

        srmdrive_tpdo1_get_drive_loop_type(_entry_buf, _entry_buf_len);
        entry_control_loop_type.string_value = _entry_buf;

        entry_dc_voltage.uint_value = srmdrive_tpdo1_get_dc_voltage();
        entry_torque.int_value = srmdrive_tpdo1_get_torque();
        entry_speed.int_value = srmdrive_tpdo1_get_speed();
    }

    private void _update_tpdo2_data()
    {
        tpdo2_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 1);
        entry_tpdo2_raw_data.ulong_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 1);

        entry_stator_current.uint_value = srmdrive_tpdo2_get_stator_current();
        entry_field_current.uint_value = srmdrive_tpdo2_get_field_current();
        entry_out_voltage.uint_value = srmdrive_tpdo2_get_out_voltage();
        entry_mech_power.uint_value = srmdrive_tpdo2_get_mech_power();
        entry_elec_power.uint_value = srmdrive_tpdo2_get_elec_power();
    }

    private void _update_tpdo3_data()
    {
        tpdo3_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 2);
        entry_tpdo3_raw_data.ulong_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 2);

        entry_pwrmodule_temp.int_value = srmdrive_tpdo3_get_pwrmodule_temp();
        entry_excmodule_temp.int_value = srmdrive_tpdo3_get_excmodule_temp();
        entry_pcb_temp.int_value = srmdrive_tpdo3_get_pcb_temp();
        entry_aw_temp.int_value = srmdrive_tpdo3_get_aw_temp();
        entry_fw_temp.int_value = srmdrive_tpdo3_get_fw_temp();
    }

    private void _update_tpdo4_data()
    {
        tpdo4_indicator.value = ucanopen_server_is_tpdo_ok(Backend.Ucanopen.server, 3);
        entry_tpdo4_raw_data.ulong_value = ucanopen_server_get_tpdo_data(Backend.Ucanopen.server, 3);
        entry_errors.uint_value = ucanopen_devices_get_error_code(Backend.Ucanopen.server);
        entry_warnings.uint_value = ucanopen_devices_get_warning_code(Backend.Ucanopen.server);
    }
}


}


