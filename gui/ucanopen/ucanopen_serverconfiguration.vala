///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_serverconfiguration.ui")]
public class ServerConfiguration : Adw.Bin
{
	[GtkChild]
	private unowned Adw.ComboRow comborowCategory;
	[GtkChild]
	private unowned Adw.ComboRow comborowEntry;
	[GtkChild]
	private unowned Gtk.Button buttonRead;

	private const size_t _categoriesCountMax = 32;
	private const size_t _categoriesLenMax = 32;
	private string _categories[_categoriesCountMax];
	private size_t _categoriesCount;
	Gtk.StringList _modelCategory;

	private const size_t _entriesCountMax = 32;
	private const size_t _entriesLenMax = 32;
	private string _entries[_entriesCountMax];
	private size_t _entriesCount;
	Gtk.StringList _modelEntry;



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

		_modelCategory = new Gtk.StringList(null);
		comborowCategory.set_model(_modelCategory);
		_modelEntry = new Gtk.StringList(null);
		comborowEntry.set_model(_modelEntry);


		size_t _categoriesCount = ucanopen_server_get_conf_categories(Backend.Ucanopen.server,
				_categories, _categoriesCountMax, _categoriesLenMax);
		for (size_t i = 0; i < _categoriesCount; ++i)
		{
			_modelCategory.append(_categories[i]);
		}

		size_t _entriesCount = ucanopen_server_get_conf_entries(Backend.Ucanopen.server,
				_categories[comborowCategory.selected], _entries, _entriesCountMax, _entriesLenMax);
		for (size_t i = 0; i < _entriesCount; ++i)
		{
			_modelEntry.append(_entries[i]);
		}

		comborowCategory.notify["selected"].connect((s,p) => {
			for (size_t i = 0; i < _entriesCount; ++i)
			{
				_modelEntry.remove(0);
			}

			_entriesCount = ucanopen_server_get_conf_entries(Backend.Ucanopen.server,
					_categories[comborowCategory.selected], _entries, _entriesCountMax, _entriesLenMax);
			for (size_t i = 0; i < _entriesCount; ++i)
			{
				_modelEntry.append(_entries[i]);
			}	
		});

		buttonRead.clicked.connect(() => {
			ucanopen_server_read(Backend.Ucanopen.server, Backend.Ucanopen.serverConfCategory,
					_categories[comborowCategory.selected], _entries[comborowEntry.selected]);
		});
	}
}

}


