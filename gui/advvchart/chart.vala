namespace AdvvChart {


public errordomain ChartError
{
	EXPORT_ERROR,
	SERIE_NOT_FOUND
}


public class Chart : Gtk.DrawingArea
{
	public Config config;

	public Background background { get; set; default = new Background(); }
	public Grid grid { get; set; default = new Grid(); }
	public Legend legend { get; set; default = new HorizontalLegend(); }


	public Chart(Config config = new Config())
	{
		this.config = config;
		this.resize.connect(() => {
			this.config.height = get_allocated_height();
			this.config.width = get_allocated_width();
		});
		//this.refresh_every(100);
	}

	construct
	{
		set_draw_func(draw_func);
	}

	void draw_func(Gtk.DrawingArea area, Cairo.Context ctx, int width, int height)
	{
		background.draw(ctx, config);
		grid.draw(ctx, config);
	}
}


}


