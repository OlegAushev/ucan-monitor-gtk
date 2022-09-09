///
///
///
namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_canbusprefs.ui")]
public class WindowCanBusPrefs : Gtk.ApplicationWindow {
	[GtkChild]

	public WindowCanBusPrefs(Gtk.Application app)
	{
		Object (application: app);
	}
}


}

    
