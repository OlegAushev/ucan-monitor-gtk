///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window.ui")]
public class Window : Gtk.ApplicationWindow
{
    [GtkChild]
    private unowned Gtk.ScrolledWindow datatables_scrolledwindow;
    [GtkChild]
    private unowned Gtk.ScrolledWindow controlpanel_scrolledwindow;

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
        default:
            message("Error: unknown server");
            break;
        }
    }
}


}


