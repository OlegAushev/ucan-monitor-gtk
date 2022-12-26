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
	[GtkChild]
	private unowned Gtk.Frame chart_area;

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
		case "LaunchPad":
			
		case "BMS Main":
			scrolledwindow_data_tables.child = new BmsMain.DataTables();
		default:
		}

		/////////////////////////////////////////////////////////////
		var chartConfig = new AdvvChart.Config();
		chartConfig.y_axis.unit = "%";
		chartConfig.y_axis.tick_interval = 25;
		chartConfig.y_axis.fixed_max = AdvvChart.cap(96);

		chartConfig.x_axis.tick_length = 60;
		chartConfig.x_axis.tick_interval = 10;
		chartConfig.x_axis.lines.visible = true;
		chartConfig.x_axis.show_fraction = false;

		chartConfig.x_axis.labels.font.size = 8;
		chartConfig.x_axis.labels.font.color = {0.0f, 1.0f, 1.0f, 1.0f};
		chartConfig.x_axis.labels.font.weight = Cairo.FontWeight.BOLD;

		chartConfig.y_axis.labels.font.size = 15;
		chartConfig.y_axis.labels.font.color = {1.0f, 0.0f, 1.0f, 0.8f};
		chartConfig.y_axis.labels.font.weight = Cairo.FontWeight.NORMAL;
		chartConfig.y_axis.labels.font.slant = Cairo.FontSlant.ITALIC;

		AdvvChart.Chart chart = new AdvvChart.Chart(chartConfig);
		chart_area.child = chart;
	}
}


}


