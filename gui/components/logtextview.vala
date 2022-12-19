///
///
///


[GtkTemplate (ui = "/gui/components/logtextview.ui")]
public class LogTextView : Adw.Bin
{
	[GtkChild]
	private unowned Gtk.TextView _textview;

	private Gtk.TextBuffer _textbuffer;

	public LogTextView() {}

	construct
	{
		_textbuffer = new Gtk.TextBuffer(null);
		_textview.buffer = _textbuffer;

		_textbuffer.create_tag("green_text", "foreground", "#33d17a", null);

		Gtk.CssProvider cssProvider = new Gtk.CssProvider();
		cssProvider.load_from_data("textview text { background-color: @view_fg_color; }".data);
		_textview.get_style_context().add_provider(cssProvider, Gtk.STYLE_PROVIDER_PRIORITY_USER);

		Timeout.add(100, update);
	}

	public bool update()
	{
		string message = string.nfill(128, '\0');;
		if (logger_get_message(message, 128))
		{
			Gtk.TextIter endIter;
			_textbuffer.get_end_iter(out endIter);

			_textview.editable = true;
			_textbuffer.insert_with_tags_by_name(ref endIter, " > ", 3, "green_text");
			_textbuffer.insert_with_tags_by_name(ref endIter, message, (int)message.length, "green_text");
			_textbuffer.insert_with_tags_by_name(ref endIter, "\n", 1, "green_text");
			_textview.editable = false;
			
			_textview.scroll_to_iter(endIter, 0, false, 0, 0);
		}

		return true;
	}
}


