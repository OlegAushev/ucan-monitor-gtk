///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_serverconfiguration.ui")]
public class ServerConfiguration : Adw.Bin
{
	[GtkChild]
	private unowned Adw.ToastOverlay toastOverlay;
	[GtkChild]
	private unowned Adw.ComboRow comborowCategory;
	[GtkChild]
	private unowned Adw.ComboRow comborowEntry;
	[GtkChild]
	private unowned Gtk.Button buttonRead;
	[GtkChild]
	private unowned Adw.EntryRow entryrowValue;
	[GtkChild]
	private unowned Gtk.Button buttonWrite;
	[GtkChild]
	private unowned Gtk.Button buttonApply;
	[GtkChild]
	private unowned Gtk.Button buttonReset;


	private const size_t _categoriesCountMax = 32;
	private const size_t _categoriesLenMax = 32;
	private string _categories[_categoriesCountMax];
	private size_t _categoriesCount;
	Gtk.StringList _categoriesModel;

	private const size_t _entriesCountMax = 32;
	private const size_t _entriesLenMax = 32;
	private string _entries[_entriesCountMax];
	private size_t _entriesCount;
	Gtk.StringList _entriesModel;


	public ServerConfiguration() {}

	construct
	{
		for (size_t i = 0; i < _categoriesCountMax; ++i)
		{
			_categories[i] = string.nfill(_categoriesLenMax, '\0');
		}

		for (size_t i = 0; i < _entriesCountMax; ++i)
		{
			_entries[i] = string.nfill(_entriesLenMax, '\0');
		}

		_categoriesModel = new Gtk.StringList(null);
		comborowCategory.set_model(_categoriesModel);
		_entriesModel = new Gtk.StringList(null);
		comborowEntry.set_model(_entriesModel);

		size_t _categoriesCount = ucanopen_server_get_conf_categories(Backend.Ucanopen.server,
				_categories, _categoriesCountMax, _categoriesLenMax);

		if (_categoriesCount == 0)
		{
			return;
		}

		for (size_t i = 0; i < _categoriesCount; ++i)
		{
			_categoriesModel.append(_categories[i]);
		}

		size_t _entriesCount = ucanopen_server_get_conf_entries(Backend.Ucanopen.server,
				_categories[comborowCategory.selected], _entries, _entriesCountMax, _entriesLenMax);
		for (size_t i = 0; i < _entriesCount; ++i)
		{
			_entriesModel.append(_entries[i]);
		}

		comborowCategory.notify["selected"].connect((s,p) => {
			for (size_t i = 0; i < _entriesCount; ++i)
			{
				_entriesModel.remove(0);
			}

			_entriesCount = ucanopen_server_get_conf_entries(Backend.Ucanopen.server,
					_categories[comborowCategory.selected], _entries, _entriesCountMax, _entriesLenMax);
			for (size_t i = 0; i < _entriesCount; ++i)
			{
				_entriesModel.append(_entries[i]);
			}	
		});

		buttonRead.clicked.connect(() => {
			ucanopen_server_read(Backend.Ucanopen.server, Backend.Ucanopen.serverConfCategory,
					_categories[comborowCategory.selected], _entries[comborowEntry.selected]);
		});

		buttonWrite.clicked.connect(() => {
			float val;
			bool isNumber = float.try_parse(entryrowValue.text, out val);
			if (isNumber && entryrowValue.text.length != 0)
			{
				ucanopen_server_write(Backend.Ucanopen.server, Backend.Ucanopen.serverConfCategory,
						_categories[comborowCategory.selected], _entries[comborowEntry.selected],
						entryrowValue.text);
			}
			else
			{
				string message = string.join(null, "Bad value: ", entryrowValue.text);
				Adw.Toast toast = new Adw.Toast(message);
				toast.timeout = 1;
				toastOverlay.add_toast(toast);
			}
		});

		buttonApply.clicked.connect(() => {
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

		buttonReset.clicked.connect(() => {
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


