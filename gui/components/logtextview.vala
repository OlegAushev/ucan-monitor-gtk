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
		Timeout.add(100, update);
	}

	public bool update()
	{
		string message = "                                                                  ";
		if (logger_get_message(message))
		{
			_textview.editable = true;
			_textview.insert_at_cursor(" > ");
			_textview.insert_at_cursor(message);
			_textview.insert_at_cursor("\n");
			_textview.editable = false;
		}
		return true;
	}
}


