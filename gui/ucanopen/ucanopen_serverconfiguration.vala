///
///
///


namespace UCanOpen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_serverconfiguration.ui")]
public class ServerConfiguration : Adw.Bin
{
	[GtkChild]
	private unowned Adw.ComboRow comborowSubcategory; 

	private const size_t _categoriesCountMax = 32;
	private const size_t _categoriesLenMax = 32;
	private string _categories[_categoriesCountMax];
	Gtk.StringList _modelSubcategory;



	public ServerConfiguration() {}

	construct
	{
		for (size_t i = 0; i < _categoriesCountMax; ++i)
		{
			_categories[i] = string.nfill(_categoriesLenMax, '\0');
		}

		size_t categoriesCount = ucanopen_server_get_conf_categories(backend.ucanopenServer, _categories, _categoriesCountMax, _categoriesLenMax);
		_modelSubcategory = new Gtk.StringList(null);
		for (size_t i = 0; i < categoriesCount; ++i)
		{
			_modelSubcategory.append(_categories[i]);
		}

		comborowSubcategory.set_model(_modelSubcategory);
	}
}

}


