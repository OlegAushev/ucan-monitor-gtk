///
///
///


namespace Ucanopen {

[GtkTemplate (ui = "/gui/ucanopen/ucanopen_selectserverwindow.ui")]
public class SelectServerWindow : Gtk.ApplicationWindow
{
    [GtkChild]
    private unowned Adw.PreferencesGroup pref_group;

    private Gtk.Button exit_button;

    private Gtk.Button ok_button;

    private Adw.ActionRow[] rows;
    private Gtk.CheckButton[] buttons;
    private Adw.ActionRow control_row;

    const int server_max_count = 10;
    private Gtk.StringList _servers;

    public SelectServerWindow(Gtk.Application app)
    {
        Object (application: app);

        exit_button.clicked.connect(() => {
            app.quit();
        });

        ok_button.clicked.connect(() => {
            for (int i = 0; i < Backend.Ucanopen.server_list.length && i < server_max_count; ++i)
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
        for (int i = 0; i < Backend.Ucanopen.server_list.length && i < server_max_count; ++i)
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

        exit_button = new Gtk.Button();
        exit_button.label = "Exit";
        exit_button.width_request = 100;
        exit_button.halign = Gtk.Align.CENTER;
        exit_button.valign = Gtk.Align.CENTER;

        ok_button = new Gtk.Button();
        ok_button.label = "Ok";
        ok_button.width_request = 100;
        ok_button.halign = Gtk.Align.CENTER;
        ok_button.valign = Gtk.Align.CENTER;
        ok_button.add_css_class("suggested-action");

        control_row = new Adw.ActionRow();
        control_row.add_prefix(exit_button);
        control_row.add_suffix(ok_button);
        pref_group.add(control_row);
    }
}

}

