///
///
///
int foo = 1000;

[GtkTemplate (ui = "/src/gui/components/slider_with_text.ui")]
public class SliderWithText : Adw.Bin
{
	[GtkChild]
	private unowned Adw.PreferencesGroup _group;

	[GtkChild]
	private unowned Gtk.Box _box;

	[GtkChild]
	private unowned Gtk.Scale _scale;

	[GtkChild]
	private unowned Gtk.SpinButton _spinbutton;

	[GtkChild]
	private unowned Gtk.Adjustment _adjustment;

	public SliderWithText()
	{

	}

	public string title
	{
		get { return _group.title; }
		set { _group.title = value; }
	}

	public double lower
	{
		get { return _adjustment.lower; }
		set { _adjustment.lower = value; }
	}

	public double upper
	{
		get { return _adjustment.upper; }
		set { _adjustment.upper = value; }
	}

	public double step
	{
		get { return _adjustment.step_increment; }
		set { _adjustment.step_increment = value; }
	}

	public double value
	{
		get { return _adjustment.value; }
		set { _adjustment.value = value; }
	}

	public int spin_width
	{
		get { return _spinbutton.width_chars; }
		set { _spinbutton.width_chars = value; }
	}

	public uint spin_digits
	{
		get { return _spinbutton.digits; }
		set { _spinbutton.digits = value; }
	} 

	public int scale_width
	{
		get { return _scale.width_request; }
		set { _scale.width_request = value; }
	} 

}
	
	
