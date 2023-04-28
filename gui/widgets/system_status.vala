///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/widgets/system_status.ui")]
public class SystemStatus : Adw.Bin
{
    [GtkChild]
    private unowned Gtk.Grid grid;

    private const int _error_name_count_max = 32;
    private const int _error_name_len_max = 64;
    private string _error_names[_error_name_count_max];
    private int _error_name_count;
    private int _error_code_byte_count;
    private Adw.PreferencesGroup[] error_bytes;
    private BoolEntry[] error_bits;
    private uint? _error_code = null;

    private const int _warning_name_count_max = 32;
    private const int _warning_name_len_max = 64;
    private string _warning_names[_warning_name_count_max];
    private int _warning_name_count;
    private int _warning_code_byte_count;
    private Adw.PreferencesGroup[] warning_bytes;
    private BoolEntry[] warning_bits;
    private uint? _warning_code = null;


    public SystemStatus() {}

    construct
    {
        // errors
        for (int i = 0; i < _error_name_count_max; ++i)
        {
            _error_names[i] = string.nfill(_error_name_len_max, '\0');
        }
        _error_name_count = ucanopen_devices_get_error_names(Backend.Ucanopen.server,
                _error_names, _error_name_count_max, _error_name_len_max);

        _error_code_byte_count = (_error_name_count + 7) / 8;

        error_bytes = new Adw.PreferencesGroup[4];
        error_bits = new BoolEntry[32];
        for (int i = 0; i < 4; ++i)
        {
            error_bytes[i] = new Adw.PreferencesGroup();
            error_bytes[i].title = @"Error Byte $i";
            error_bytes[i].width_request = 280;
            error_bytes[i].hexpand = false;
            grid.attach(error_bytes[i], i, 0);

            for (int j = 0; j < 8; ++j)
            {
                error_bits[8*i+j] = new BoolEntry();	
                error_bits[8*i+j].title = _error_names[8*i+j];

                error_bits[8*i+j].value_chars = 2;
                error_bits[8*i+j].true_text = "1";
                error_bits[8*i+j].false_text = "0";
                error_bits[8*i+j].true_css_class = "error";
                error_bits[8*i+j].false_css_class = "success";

                error_bytes[i].add(error_bits[8*i+j]);
            }
        }

        // warnings
        for (int i = 0; i < _warning_name_count_max; ++i)
        {
            _warning_names[i] = string.nfill(_warning_name_len_max, '\0');
        }
        _warning_name_count = ucanopen_devices_get_warning_names(Backend.Ucanopen.server,
                _warning_names, _warning_name_count_max, _warning_name_len_max);

        _warning_code_byte_count = (_warning_name_count + 7) / 8;

        warning_bytes = new Adw.PreferencesGroup[4];
        warning_bits = new BoolEntry[32];
        for (int i = 0; i < 4; ++i)
        {
            warning_bytes[i] = new Adw.PreferencesGroup();
            warning_bytes[i].title = @"Warning Byte $i";
            warning_bytes[i].width_request = 280;
            warning_bytes[i].hexpand = false;
            grid.attach(warning_bytes[i], i, 1);

            for (int j = 0; j < 8; ++j)
            {
                warning_bits[8*i+j] = new BoolEntry();	
                warning_bits[8*i+j].title = _warning_names[8*i+j];

                warning_bits[8*i+j].value_chars = 2;
                warning_bits[8*i+j].true_text = "1";
                warning_bits[8*i+j].false_text = "0";
                warning_bits[8*i+j].true_css_class = "error";
                warning_bits[8*i+j].false_css_class = "success";

                warning_bytes[i].add(warning_bits[8*i+j]);
            }
        }

        Timeout.add(50, update);
    }
    
    public bool update()
    {
        _updateErrors();
        _updateWarnings();
        return true;
    }

    private void _updateErrors()
    {
        uint error_code = ucanopen_devices_get_error_code(Backend.Ucanopen.server);
        if (error_code == _error_code)
        {
            return;
        }

        _error_code = error_code;

        for (int i = 0; i < _error_name_count; ++i)
        {
            if ((_error_code & (1 << i)) != 0)
            {
                error_bits[i].value = true;
            }
            else
            {
                error_bits[i].value = false;
            }
        }
    }

    private void _updateWarnings()
    {
        uint warning_code = ucanopen_devices_get_warning_code(Backend.Ucanopen.server);
        if (warning_code == _warning_code)
        {
            return;
        }

        _warning_code = warning_code;

        for (int i = 0; i < _warning_name_count; ++i)
        {
            if ((_warning_code & (1 << i)) != 0)
            {
                warning_bits[i].value = true;
            }
            else
            {
                warning_bits[i].value = false;
            }
        }
    }
}


}


