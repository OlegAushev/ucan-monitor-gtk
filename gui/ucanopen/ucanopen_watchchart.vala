///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_watchchart.ui")]
public class WatchChart : Adw.Bin
{
	[GtkChild]
	private unowned Adw.ComboRow comborow_watch_name;
	[GtkChild]
	private unowned Gtk.Frame frame_chart;

	public WatchChart() {}

	construct
	{
		/////////////////////////////////////////////////////////////
		var rss = new AdvvChart.Series("RSS",  new AdvvChart.Line());
		rss.line.color = {0.88f, 0.11f, 0.14f, 1.0f};


		var chartConfig = new AdvvChart.Config();
		chartConfig.y_axis.unit = "";
		chartConfig.y_axis.lines.visible = true;

		chartConfig.x_axis.tick_length = 60;
		chartConfig.x_axis.tick_interval = 1;
		chartConfig.x_axis.lines.visible = true;
		chartConfig.x_axis.show_fraction = false;

		chartConfig.x_axis.labels.font.size = 12;
		chartConfig.x_axis.labels.font.color = {0.47f, 0.68f, 0.96f, 1.0f};
		chartConfig.x_axis.labels.font.weight = Cairo.FontWeight.NORMAL;

		chartConfig.y_axis.labels.font.size = 12;
		chartConfig.y_axis.labels.font.color = {0.47f, 0.68f, 0.96f, 1.0f};
		chartConfig.y_axis.labels.font.weight = Cairo.FontWeight.NORMAL;
		//chartConfig.y_axis.labels.font.slant = Cairo.FontSlant.ITALIC;

		AdvvChart.Chart chart = new AdvvChart.Chart(chartConfig);
		chart.add_series(rss);


		double rss_value = 200.0;
		Timeout.add(10, () => {
			/*if (Random.double_range(0.0, 1.0) > 0.13)
			{
				var new_value = Random.double_range(-50, 50.0);
				if (rss_value + new_value > 0) rss_value += new_value;
			}*/
			
			rss_value = 200 + 100 * Math.sin(0.000001 * GLib.get_real_time()) + 50 * Math.cos(0.000004 * GLib.get_real_time());
			rss.add(rss_value);
			return true;
		});

		frame_chart.child = chart;
	}



}


}


