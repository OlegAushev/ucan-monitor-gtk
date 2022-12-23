///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window_selectserver.ui")]
public class WindowSelectServer : Gtk.ApplicationWindow
{
	[GtkChild]
	private unowned Gtk.Button button_exit;
	[GtkChild]
	private unowned Gtk.Button button_ok;

	public WindowSelectServer(Gtk.Application app)
	{
		Object (application: app);

		button_exit.clicked.connect(() => {
			app.quit();
		});

		button_ok.clicked.connect(() => {
			close();
			app.activate_action("create_main_window", null);
		});
	}

}


}