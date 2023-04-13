///
///
///


[GtkTemplate (ui = "/gui/components/check_entry.ui")]
public class CheckEntry : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.CheckButton _checkbutton;

	public CheckEntry() {}

	construct {}

	public bool value
	{
		get { return _checkbutton.active; }
		set { _checkbutton.active = value; }
	}
}