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
	uint i = 0;
	public bool update()
	{
		string message = string.nfill(128, '\0');;
		if (logger_get_message(message, 128))
		{
			Gtk.TextIter endIter;
			_textbuffer.get_end_iter(out endIter);

			_textview.editable = true;
			_textbuffer.insert(ref endIter, " > ", 3);
			_textbuffer.insert(ref endIter, message, (int)message.length);
			_textbuffer.insert(ref endIter, "\n", 1);
			_textview.editable = false;
			
			_textview.scroll_to_iter(endIter, 0, false, 0, 0);
		}

		return true;
	}
}


