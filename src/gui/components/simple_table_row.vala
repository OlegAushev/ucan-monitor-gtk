///
///
///


[GtkTemplate (ui = "/src/gui/components/simple_table_row.ui")]
public class SimpleTableRow : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.Entry _entry;

	public SimpleTableRow()
	{
		_entry.set_alignment(1);
	}

	construct
	{
		
	}

	public string entry_text
	{
		get { return _entry.text; }
		set { _entry.text = value; }
	}
}


