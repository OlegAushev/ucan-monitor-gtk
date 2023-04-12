///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/window.ui")]
public class Window : Gtk.ApplicationWindow
{
	[GtkChild]
	private unowned Gtk.ScrolledWindow scrolledwindow_data_tables;
	[GtkChild]
	private unowned Gtk.ScrolledWindow scrolledwindow_control_panel;

	public Window(Gtk.Application app)
	{
		Object (application: app);
	}

	construct
	{
		switch (Backend.Ucanopen.server)
		{
			case "SRM-Drive-80":
				scrolledwindow_control_panel.child = new SrmDrive.ControlPanel();
				scrolledwindow_control_panel.child.add_css_class("background");
				scrolledwindow_data_tables.child = new SrmDrive.DataTables();
				break;
			case "CRD600":
				scrolledwindow_control_panel.child = new Crd600.ControlPanel();
				scrolledwindow_control_panel.child.add_css_class("background");
				scrolledwindow_data_tables.child = new Crd600.DataTables();
				break;
			case "LaunchPad":
				scrolledwindow_control_panel.child = new LaunchPad.ControlPanel();
				scrolledwindow_control_panel.child.add_css_class("background");
				scrolledwindow_data_tables.child = new LaunchPad.DataTables();
				break;
			case "BMS-Main":
				scrolledwindow_data_tables.child = new BmsMain.DataTables();
				break;
			default:
		}
	}
}


}


