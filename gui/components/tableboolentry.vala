///
///
///


[GtkTemplate (ui = "/gui/components/tableboolentry.ui")]
public class TableBoolEntry : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.CheckButton _check;

	public TableBoolEntry() {}

	construct {}

	public bool value
	{
		get { return _check.active; }
		set { _check.active = value; }
	}
}