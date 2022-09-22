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
}


