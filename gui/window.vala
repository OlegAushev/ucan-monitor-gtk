/* window.vala
 *
 * Copyright 2022 Oleg Aushev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
		case "SRM Drive":
			scrolledwindow_control_panel.child = new SrmDrive.ControlPanel();
			scrolledwindow_control_panel.child.add_css_class("background");
			scrolledwindow_data_tables.child = new SrmDrive.DataTables();
			break;
		case "LaunchPad":
			scrolledwindow_data_tables.child = new LaunchPad.DataTables();
			break;
		case "BMS Main":
			scrolledwindow_data_tables.child = new BmsMain.DataTables();
			break;
		default:
		}
	}
}


}


