///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_serverconfiguration.ui")]
public class ServerConfiguration : Adw.Bin
{
	[GtkChild]
	private unowned Adw.ToastOverlay toast_overlay;
	[GtkChild]
	private unowned Adw.ComboRow comborow_category;
	[GtkChild]
	private unowned Adw.ComboRow comborow_entry;
	[GtkChild]
	private unowned Gtk.Button button_read;
	[GtkChild]
	private unowned Adw.EntryRow entryrow_value;
	[GtkChild]
	private unowned Gtk.Button button_write;
	[GtkChild]
	private unowned Gtk.Button button_apply;
	[GtkChild]
	private unowned Gtk.Button button_reset;


	private const size_t _categories_count_max = 32;
	private const size_t _categories_len_max = 32;
	private string _categories[_categories_count_max];
	private size_t _categories_count;
	private Gtk.StringList _categories_model;

	private const size_t _entries_count_max = 32;
	private const size_t _entries_len_max = 32;
	private string _entries[_entries_count_max];
	private size_t _entries_count;
	private Gtk.StringList _entries_model;


	public ServerConfiguration() {}

	construct
	{
		for (size_t i = 0; i < _categories_count_max; ++i)
		{
			_categories[i] = string.nfill(_categories_len_max, '\0');
		}

		for (size_t i = 0; i < _entries_count_max; ++i)
		{
			_entries[i] = string.nfill(_entries_len_max, '\0');
		}

		_categories_model = new Gtk.StringList(null);
		comborow_category.model = _categories_model;
		_entries_model = new Gtk.StringList(null);
		comborow_entry.model = _entries_model;

		size_t _categories_count = ucanopen_server_get_config_categories(Backend.Ucanopen.server,
				_categories, _categories_count_max, _categories_len_max);

		if (_categories_count == 0)
		{
			return;
		}

		for (size_t i = 0; i < _categories_count; ++i)
		{
			_categories_model.append(_categories[i]);
		}

		size_t _entries_count = ucanopen_server_get_config_entries(Backend.Ucanopen.server,
				_categories[comborow_category.selected], _entries, _entries_count_max, _entries_len_max);
		for (size_t i = 0; i < _entries_count; ++i)
		{
			_entries_model.append(_entries[i]);
		}

		comborow_category.notify["selected"].connect((s,p) => {
			for (size_t i = 0; i < _entries_count; ++i)
			{
				_entries_model.remove(0);
			}

			_entries_count = ucanopen_server_get_config_entries(Backend.Ucanopen.server,
					_categories[comborow_category.selected], _entries, _entries_count_max, _entries_len_max);
			for (size_t i = 0; i < _entries_count; ++i)
			{
				_entries_model.append(_entries[i]);
			}	
		});

		button_read.clicked.connect(() => {
			ucanopen_server_read(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
					_categories[comborow_category.selected], _entries[comborow_entry.selected]);
		});

		button_write.clicked.connect(() => {
			float val;
			bool is_number = float.try_parse(entryrow_value.text, out val);
			if (is_number && entryrow_value.text.length != 0)
			{
				ucanopen_server_write(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
						_categories[comborow_category.selected], _entries[comborow_entry.selected],
						entryrow_value.text);
			}
			else
			{
				string message = string.join(null, "Bad value: ", entryrow_value.text);
				Adw.Toast toast = new Adw.Toast(message);
				toast.timeout = 1;
				toast_overlay.add_toast(toast);
			}
		});

		button_apply.clicked.connect(() => {
			Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
					"Warning!",
					"All configuration parameters will be applied.");
			dialog.add_response("cancel", "Cancel");
			dialog.add_response("continue", "Continue");
			dialog.set_response_appearance("cancel", DESTRUCTIVE);
			dialog.set_response_appearance("continue", SUGGESTED);
			dialog.response["continue"].connect(() => {
				ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "APPLY PARAMETERS");
			});
			dialog.present();	
		});

		button_reset.clicked.connect(() => {
			Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
					"Warning!",
					"All configuration parameters will be reset to default values.");
			dialog.add_response("cancel", "Cancel");
			dialog.add_response("continue", "Continue");
			dialog.set_response_appearance("cancel", DESTRUCTIVE);
			dialog.set_response_appearance("continue", SUGGESTED);
			dialog.response["continue"].connect(() => {
				ucanopen_server_exec(Backend.Ucanopen.server, "SYSTEM CONTROL", "SYSTEM CONTROL", "RESET PARAMETERS");
			});
			dialog.present();		
		});
	}
}


}


