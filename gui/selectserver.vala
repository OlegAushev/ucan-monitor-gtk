///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/selectserver.ui")]
public class WindowSelectServer : Gtk.ApplicationWindow
{
	[GtkChild]
	private unowned Gtk.Button button_exit;
	[GtkChild]
	private unowned Gtk.Button button_ok;

	[GtkChild]
	private unowned Adw.ComboRow comborow_servers;
	
	private Gtk.StringList _servers;

	public WindowSelectServer(Gtk.Application app)
	{
		Object (application: app);

		button_exit.clicked.connect(() => {
			app.quit();
		});

		button_ok.clicked.connect(() => {
			Backend.Ucanopen.server = Backend.Ucanopen.server_list[comborow_servers.selected];
			close();
			app.activate_action("create_main_window", null);
		});
	}

	construct
	{
		_servers = new Gtk.StringList(null);
		for(size_t i = 0; i < Backend.Ucanopen.server_list.length; ++i)
		{
			_servers.append(Backend.Ucanopen.server_list[i]);
		}
		comborow_servers.model = _servers;
	}

}


}