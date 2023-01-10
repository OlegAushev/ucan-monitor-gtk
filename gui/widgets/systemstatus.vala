///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/widgets/systemstatus.ui")]
public class SystemStatus : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Grid grid;

	private const size_t _error_name_count_max = 32;
	private const size_t _error_name_len_max = 64;
	private string _error_names[_error_name_count_max];
	private size_t _error_name_count;
	private Gtk.StringList _error_names_list;
	private size_t _error_code_byte_count;

	private Adw.PreferencesGroup[] error_bytes;//[4];
	private TableBoolEntry[,] error_bits;//[4,8];

	public SystemStatus() {}

	construct
	{
		for (size_t i = 0; i < _error_name_count_max; ++i)
		{
			_error_names[i] = string.nfill(_error_name_len_max, '\0');
		}
		_error_name_count = ucanopen_devices_get_error_names(Backend.Ucanopen.server,
				_error_names, _error_name_count_max, _error_name_len_max);

		_error_code_byte_count = (_error_name_count + 7) / 8;


		error_bytes = new Adw.PreferencesGroup[4];
		error_bits = new TableBoolEntry[4,8];
		for (int i = 0; i < 4; ++i)
		{
			error_bytes[i] = new Adw.PreferencesGroup();
			error_bytes[i].title = @"Error Byte $i";
			grid.attach(error_bytes[i], i, 0);

			for (int j = 0; j < 8; ++j)
			{
				error_bits[i,j] = new TableBoolEntry();
				error_bytes[i].add(error_bits[i,j]);
			}
		}



		Timeout.add(50, update);
	}
	
	public bool update()
	{
		
		return true;
	}
}


}


