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
	private unowned Gtk.Button button_refresh_about;
	[GtkChild]
	private unowned Gtk.Label label_device_name;
	[GtkChild]
	private unowned Gtk.Label label_hardware_version;
	[GtkChild]
	private unowned Gtk.Label label_firmware_version;
	[GtkChild]
	private unowned Gtk.Label label_serial_number;

	[GtkChild]
	private unowned Adw.ComboRow comborow_category;
	[GtkChild]
	private unowned Adw.ComboRow comborow_object;
	[GtkChild]
	private unowned Gtk.Button button_refresh_value;
	[GtkChild]
	private unowned Adw.EntryRow entryrow_value;
	//[GtkChild]
	//private unowned Gtk.Button button_write;
	
	[GtkChild]
	private unowned Gtk.Button button_apply;
	[GtkChild]
	private unowned Gtk.Button button_restore;


	private const size_t _category_count_max = 32;
	private const size_t _category_str_size = 32;
	private string _categories[_category_count_max];
	private Gtk.StringList _categories_model;

	private const size_t _object_count_max = 32;
	private const size_t _object_str_size = 32;
	private string _objects[_object_count_max];
	private Gtk.StringList _objects_model;


	public ServerConfiguration() {}

	construct
	{
		button_refresh_about.clicked.connect(() => {
			string buf = string.nfill(32, '0');
			ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "device_name", 500, buf, 32);
			label_device_name.label = buf;
			ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "hardware_version", 500, buf, 32);
			label_hardware_version.label = buf;
			ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "firmware_version", 500, buf, 32);
			label_firmware_version.label = buf;
			ucanopen_server_read_numval(Backend.Ucanopen.server, "sys", "info", "serial_number", 500, buf, 32);
			label_serial_number.label = buf;
		});


		for (size_t i = 0; i < _category_count_max; ++i)
		{
			_categories[i] = string.nfill(_category_str_size, '\0');
		}

		for (size_t i = 0; i < _object_count_max; ++i)
		{
			_objects[i] = string.nfill(_object_str_size, '\0');
		}

		_categories_model = new Gtk.StringList(null);
		comborow_category.model = _categories_model;
		_objects_model = new Gtk.StringList(null);
		comborow_object.model = _objects_model;

		size_t category_count = ucanopen_server_get_config_categories(Backend.Ucanopen.server,
				_categories, _category_count_max, _category_str_size);

		if (category_count != 0)
		{
			for (size_t i = 0; i < category_count; ++i)
			{
				_categories_model.append(_categories[i]);
			}

			size_t object_count = ucanopen_server_get_config_objects(Backend.Ucanopen.server,
					_categories[comborow_category.selected], _objects, _object_count_max, _object_str_size);
			for (size_t i = 0; i < object_count; ++i)
			{
				_objects_model.append(_objects[i]);
			}

			comborow_category.notify["selected"].connect(on_category_selected);
			comborow_object.notify["selected"].connect(on_object_selected);
			button_refresh_value.clicked.connect(on_value_refresh);
			entryrow_value.apply.connect(on_value_write);
		}

		button_apply.clicked.connect(() => {
			Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
					"Warning!",
					"All configuration parameters will be applied.");
			dialog.add_response("cancel", "Cancel");
			dialog.add_response("continue", "Continue");
			dialog.set_response_appearance("cancel", DESTRUCTIVE);
			dialog.set_response_appearance("continue", SUGGESTED);
			dialog.response["continue"].connect(() => {
				ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "save_all_parameters");
			});
			dialog.present();	
		});

		button_restore.clicked.connect(() => {
			Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
					"Warning!",
					"Default configuration parameters will be restored.");
			dialog.add_response("cancel", "Cancel");
			dialog.add_response("continue", "Continue");
			dialog.set_response_appearance("cancel", DESTRUCTIVE);
			dialog.set_response_appearance("continue", SUGGESTED);
			dialog.response["continue"].connect(() => {
				ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "restore_all_default_parameters");
			});
			dialog.present();		
		});
	}

	void on_object_selected()
	{
		string buf = string.nfill(32, '\0');
		ucanopen_server_read_numval(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
				_categories[comborow_category.selected], _objects[comborow_object.selected],
				500, buf, 32);
		entryrow_value.text = buf;
	}

	void on_category_selected()
	{
		comborow_object.notify["selected"].disconnect(on_object_selected);
		_objects_model.splice(0, _objects_model.get_n_items(), null);
		size_t object_count = ucanopen_server_get_config_objects(Backend.Ucanopen.server,
				_categories[comborow_category.selected], _objects, _object_count_max, _object_str_size);
		for (size_t i = 0; i < object_count; ++i)
		{
			_objects_model.append(_objects[i]);
		}
		comborow_object.notify["selected"].connect(on_object_selected);
		on_object_selected();
	}

	void on_value_refresh()
	{
		string buf = string.nfill(32, '\0');
		ucanopen_server_read_numval(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
				_categories[comborow_category.selected], _objects[comborow_object.selected],
				500, buf, 32);
		entryrow_value.text = buf;
	}

	void on_value_write()
	{
		float val;
		bool is_number = float.try_parse(entryrow_value.text, out val);
		if (is_number && entryrow_value.text.length != 0)
		{
			ucanopen_server_write(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
					_categories[comborow_category.selected], _objects[comborow_object.selected],
					entryrow_value.text);
		}
		else
		{
			string message = string.join(null, "Bad value: ", entryrow_value.text);
			Adw.Toast toast = new Adw.Toast(message);
			toast.timeout = 1;
			toast_overlay.add_toast(toast);
		}
	}
}


}


