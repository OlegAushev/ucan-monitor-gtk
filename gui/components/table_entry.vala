///
///
///


[GtkTemplate (ui = "/gui/components/table_entry.ui")]
public class TableEntry : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.Entry _entry;

	public TableEntry() {}

	construct {}

	public string entry_text
	{
		get { return _entry.text; }
		set { _entry.text = value; }
	}

	public int entry_width
	{
		get { return _entry.width_request; }
		set { _entry.width_request = value; }
	}

	public int entry_chars
	{
		get { return _entry.max_width_chars; }
		set { _entry.max_width_chars = value; }
	}

	public float entry_xalign
	{
		get { return _entry.xalign; }
		set { _entry.xalign = value; }
	}

	public void entry_add_css_class (string css_class)
	{
		_entry.add_css_class(css_class);
	}

	public void entry_remove_css_class (string css_class)
	{
		_entry.remove_css_class(css_class);
	}
}


