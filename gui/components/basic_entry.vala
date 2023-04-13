///
///
///


[GtkTemplate (ui = "/gui/components/basic_entry.ui")]
public class BasicEntry : Adw.ActionRow {
	[GtkChild]
	private unowned Gtk.Entry _entry;

	public BasicEntry() {}

	construct {}

	public string string_value {
		get { return _entry.text; }
		set { _entry.text = value; }
	}

    public string float_format { get; set; default = "%.2f"; }
    public string int_format { get; set; default = "%i"; }
    public string uint_format { get; set; default = "%u"; }
    public string long_format { get; set; default = "%li"; }
    public string ulong_format { get; set; default = "%lu"; }

    public float float_value {
        private get { return 0; }
        set {
            _entry.text = value.to_string(float_format);
        }
    }

    public int int_value {
        private get { return 0; }
        set {
            _entry.text = value.to_string(int_format);
        }
    }

    public uint uint_value {
        private get { return 0; }
        set {
            _entry.text = value.to_string(uint_format);
        }
    }

    public long long_value {
        private get { return 0; }
        set {
            _entry.text = value.to_string(long_format);
        }
    }

    public ulong ulong_value {
        private get { return 0; }
        set {
            _entry.text = value.to_string(ulong_format);
        }
    }

	public int value_width {
		get { return _entry.width_request; }
		set { _entry.width_request = value; }
	}

	public int value_chars {
		get { return _entry.max_width_chars; }
		set { _entry.max_width_chars = value; }
	}

	public float value_xalign {
		get { return _entry.xalign; }
		set { _entry.xalign = value; }
	}

	public void entry_add_css_class (string css_class) {
		_entry.add_css_class(css_class);
	}

	public void entry_remove_css_class (string css_class) {
		_entry.remove_css_class(css_class);
	}
}


