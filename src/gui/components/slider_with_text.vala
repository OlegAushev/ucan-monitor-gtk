///
///
///
int foo = 1000;

[GtkTemplate (ui = "/src/gui/components/slider_with_text.ui")]
public class SliderWithText : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.Grid _grid_root;

	[GtkChild]
	private unowned Gtk.Label _label;

	[GtkChild]
	private unowned Gtk.Scale _scale;

	[GtkChild]
	private unowned Gtk.SpinButton _spinbutton;

	[GtkChild]
	private unowned Gtk.Adjustment _adjustment;

	public SliderWithText()
	{

	}

	public string label
	{
		get { return _label.label; }
		set { _label.label = value; }
	}

}
	
	
