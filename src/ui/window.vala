namespace CanMonitor {

[GtkTemplate (ui = "/src/ui/assets/window.ui")]
public class Window : Gtk.ApplicationWindow {
    [GtkChild]
    private unowned Gtk.ScrolledWindow datatables_scrolledwindow;
    [GtkChild]
    private unowned Gtk.ScrolledWindow controlpanel_scrolledwindow;
    [GtkChild]
    private unowned LogTextView log_textview;
    [GtkChild]
    private unowned Adw.ViewStack viewstack;
    [GtkChild]
    private unowned Gtk.Paned flap_content;


    public Window(Gtk.Application app) {
        Object (application: app);
    }

    construct {
        switch (Backend.Ucanopen.server) {
        case "SRM-Drive-80":
            controlpanel_scrolledwindow.child = new SrmDrive.ControlPanel();
            controlpanel_scrolledwindow.child.add_css_class("background");
            datatables_scrolledwindow.child = new SrmDrive.DataTables();
            break;
        case "CRD600":
            controlpanel_scrolledwindow.child = new Crd600.ControlPanel();
            controlpanel_scrolledwindow.child.add_css_class("background");
            datatables_scrolledwindow.child = new Crd600.DataTables();
            break;
        case "LaunchPad":
            controlpanel_scrolledwindow.child = new LaunchPad.ControlPanel();
            controlpanel_scrolledwindow.child.add_css_class("background");
            datatables_scrolledwindow.child = new LaunchPad.DataTables();
            break;
        case "BMS-Main":
            datatables_scrolledwindow.child = new BmsMain.DataTables();
            break;
        case "ATV-VCU":
            controlpanel_scrolledwindow.child = new AtvVcu.ControlPanel();
            controlpanel_scrolledwindow.child.add_css_class("background");
            datatables_scrolledwindow.child = new AtvVcu.DataTables();
            break;
        default:
            message("Error: unknown server");
            break;
        }

        viewstack.pages.selection_changed.connect((position, n_items) => {
            if (viewstack.pages.is_selected(2) || viewstack.pages.is_selected(3)) {
                log_textview.visible = true;
                flap_content.position = flap_content.max_position / 2;
            } else {
                log_textview.visible = false;
            }
        });
    }
}

}
