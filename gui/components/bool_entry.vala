///
///
///


[GtkTemplate (ui = "/gui/components/bool_entry.ui")]
public class BoolEntry : Adw.ActionRow
{
	[GtkChild]
	private unowned Gtk.Entry _entry;

	private bool _value;
	private string _true_text = "true";
	private string _false_text = "false";
	private string _true_css_class = "opaque";
	private string _false_css_class = "opaque";

	public BoolEntry() {}

	construct {}

	public bool value
	{
		get { return _value; }
		set
		{ 
			_value = value;
			if (_value)
			{
				_entry.remove_css_class(_false_css_class);
				_entry.text = _true_text;
				_entry.add_css_class(_true_css_class);
			}
			else
			{
				_entry.remove_css_class(_true_css_class);
				_entry.text = _false_text;
				_entry.add_css_class(_false_css_class);
			}
		}
	}

	public string true_text
	{
		set { _true_text = value; }
	}

	public string false_text
	{
		set { _false_text = value; }
	}

	public string true_css_class
	{
		set { _true_css_class = value; }
	}

	public string false_css_class
	{
		set { _false_css_class = value; }
	}

	public int value_width
	{
		get { return _entry.width_request; }
		set { _entry.width_request = value; }
	}

	public int value_chars
	{
		get { return _entry.max_width_chars; }
		set { _entry.max_width_chars = value; }
	}

	public float value_xalign
	{
		get { return _entry.xalign; }
		set { _entry.xalign = value; }
	}
}


