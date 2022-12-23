///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_selectserver.ui")]
public class WindowSelectServer : Gtk.ApplicationWindow
{
	public WindowSelectServer(Gtk.Application app)
	{
		Object (application: app);
	}
}


}