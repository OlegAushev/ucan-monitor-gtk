///
///
///


[GtkTemplate (ui = "/gui/components/tablecheckentry.ui")]
public class TableCheckEntry : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.CheckButton _checkbutton;

	public TableCheckEntry() {}

	construct {}

	public bool value
	{
		get { return _checkbutton.active; }
		set { _checkbutton.active = value; }
	}
}