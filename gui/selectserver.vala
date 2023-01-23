///
///
///


namespace CanMonitor {

[GtkTemplate (ui = "/gui/selectserver.ui")]
public class WindowSelectServer : Gtk.ApplicationWindow
{
	[GtkChild]
	private unowned Adw.PreferencesGroup pref_group;

	private Gtk.Button button_exit;

	private Gtk.Button button_ok;

	private Adw.ActionRow[] rows;
	private Gtk.CheckButton[] buttons;
	private Adw.ActionRow control_row;

	const uint server_max_count = 10;
	private Gtk.StringList _servers;

	public WindowSelectServer(Gtk.Application app)
	{
		Object (application: app);

		button_exit.clicked.connect(() => {
			app.quit();
		});

		button_ok.clicked.connect(() => {
			for (size_t i = 0; i < Backend.Ucanopen.server_list.length && i < server_max_count; ++i)
			{
				if (buttons[i].active)
				{
					Backend.Ucanopen.server = Backend.Ucanopen.server_list[i];
				}
			}
			close();
			app.activate_action("create_main_window", null);
		});
	}

	construct
	{
		rows = new Adw.ActionRow[server_max_count];
		buttons = new Gtk.CheckButton[server_max_count];

		_servers = new Gtk.StringList(null);
		for (size_t i = 0; i < Backend.Ucanopen.server_list.length && i < server_max_count; ++i)
		{
			_servers.append(Backend.Ucanopen.server_list[i]);

			rows[i] = new Adw.ActionRow();
			buttons[i] = new Gtk.CheckButton();
			if (i == 0)
			{
				buttons[i].active = true;
			}
			else
			{
				buttons[i].group = buttons[0];
			}
			
			rows[i].add_prefix(buttons[i]);
			rows[i].title = Backend.Ucanopen.server_list[i];
			rows[i].activatable_widget = buttons[i];

			pref_group.add(rows[i]);
		}

		button_exit = new Gtk.Button();
		button_exit.label = "Exit";
		button_exit.width_request = 100;
		button_exit.halign = Gtk.Align.CENTER;
		button_exit.valign = Gtk.Align.CENTER;

		button_ok = new Gtk.Button();
		button_ok.label = "Ok";
		button_ok.width_request = 100;
		button_ok.halign = Gtk.Align.CENTER;
		button_ok.valign = Gtk.Align.CENTER;
		button_ok.add_css_class("suggested-action");

		control_row = new Adw.ActionRow();
		control_row.add_prefix(button_exit);
		control_row.add_suffix(button_ok);
		pref_group.add(control_row);
	}
}

}

