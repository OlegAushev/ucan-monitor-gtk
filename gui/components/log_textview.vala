///
///
///


[GtkTemplate (ui = "/gui/components/log_textview.ui")]
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

		_textbuffer.create_tag("gray_text", "foreground", "#c0bfbc", null);
        _textbuffer.create_tag("green_text", "foreground", "#2ec27e", null);
        _textbuffer.create_tag("red_text", "foreground", "#e01b24", null);
        _textbuffer.create_tag("yellow_text", "foreground", "#e5a50a", null);
        _textbuffer.create_tag("blue_text", "foreground", "#78aeed", null);
        _textbuffer.create_tag("purple_text", "foreground", "#c061cb", null);

		Gtk.CssProvider css_provider = new Gtk.CssProvider();
		css_provider.load_from_data("textview text { background-color: #1e1e1e; }".data);
		_textview.get_style_context().add_provider(css_provider, Gtk.STYLE_PROVIDER_PRIORITY_USER);

		Timeout.add(100, update);
	}

	public bool update()
	{
		string message = string.nfill(256, '\0');
		if (log_get_message(message, 256))
		{
			Gtk.TextIter end_iter;
			_textbuffer.get_end_iter(out end_iter);
            int line = end_iter.get_line();

			_textview.editable = true;
			_textbuffer.insert_with_tags_by_name(ref end_iter, " ", 1, "gray_text");
			_textbuffer.insert_with_tags_by_name(ref end_iter, message, (int)message.length, "gray_text");
			_textbuffer.insert_with_tags_by_name(ref end_iter, "\n", 1, "gray_text");
			_textview.editable = false;
			
			_textview.scroll_to_iter(end_iter, 0, false, 0, 0);

            // colorize
            Gtk.TextIter line_begin_iter;
            _textbuffer.get_iter_at_line(out line_begin_iter, line);
            Gtk.TextIter searchres_begin = line_begin_iter;
            Gtk.TextIter searchres_end = line_begin_iter;
            
            if (line_begin_iter.forward_search("OK", Gtk.TextSearchFlags.TEXT_ONLY, out searchres_begin, out searchres_end, null)) {
                _textbuffer.apply_tag_by_name("green_text", searchres_begin, searchres_end);
            } else if (line_begin_iter.forward_search("FAILED", Gtk.TextSearchFlags.TEXT_ONLY, out searchres_begin, out searchres_end, null)) {
                _textbuffer.apply_tag_by_name("red_text", searchres_begin, searchres_end);
            } else if (line_begin_iter.forward_search("WARN", Gtk.TextSearchFlags.TEXT_ONLY, out searchres_begin, out searchres_end, null)) {
                _textbuffer.apply_tag_by_name("yellow_text", searchres_begin, searchres_end);
            }

            if (line_begin_iter.forward_search("@syslog:", Gtk.TextSearchFlags.TEXT_ONLY, out searchres_begin, out searchres_end, null)) {
                _textbuffer.apply_tag_by_name("purple_text", searchres_begin, searchres_end);
            }

            Gtk.TextIter aux_iter1 = line_begin_iter;
            Gtk.TextIter aux_iter2 = line_begin_iter;
            if ((line_begin_iter.forward_search("{", Gtk.TextSearchFlags.TEXT_ONLY, out searchres_begin, out aux_iter1, null))
             && (line_begin_iter.forward_search("}", Gtk.TextSearchFlags.TEXT_ONLY, out aux_iter2, out searchres_end, null)))
            {
                _textbuffer.apply_tag_by_name("blue_text", searchres_begin, searchres_end);
            }
		}

		return true;
	}
}


