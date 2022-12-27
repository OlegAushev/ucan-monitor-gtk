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
	public SeriesPool series;

	private uint _source_timeout = 0;
        private double _play_ratio = 1.0;
	private int64 _prev_time;

	public Chart(Config config = new Config())
	{
		this.config = config;
		this.resize.connect(() => {
			this.config.height = get_allocated_height();
			this.config.width = get_allocated_width();
		});

		refresh_every(100);

		series = new SeriesPool(this);

		this.destroy.connect(() =>
		{
			refresh_every(-1);
			remove_all_series();
		});
	}

	construct
	{
		set_draw_func(draw_func);
	}

	void draw_func(Gtk.DrawingArea area, Cairo.Context ctx, int width, int height)
	{
		config.configure(ctx, legend);

		background.draw(ctx, config);
		grid.draw(ctx, config);
		if (legend != null) legend.draw(ctx, config);

		var boundaries = config.boundaries();
		foreach (Drawable series_ in series)
		{
			ctx.rectangle(boundaries.x.min, boundaries.y.min, boundaries.x.max, boundaries.y.max);
			ctx.clip();
			series_.draw(ctx, config);
		}
	}

	public void add_series(Series series_)
	{
		series.register(series_);
	}
    
	public void remove_series(Series series_){
		series.remove_series(series_);
	}
    
	public void remove_all_series()
	{
		series.remove_all();
	}

	public void refresh_every(int ms, double play_ratio = 1.0)
	{
		_play_ratio = play_ratio;
		if (_source_timeout != 0)
		{
			GLib.Source.remove(_source_timeout); 
			_source_timeout = 0;
		}
		if(ms > 0)
		{
			_prev_time = GLib.get_monotonic_time() / config.time.conv_us;
			_source_timeout = Timeout.add(ms, () => 
			{
				if(_play_ratio != 0.0)
				{
					var now = GLib.get_monotonic_time() / config.time.conv_us;
					config.time.current += (int64)((now - _prev_time));
					_prev_time = now;
				}
				queue_draw();
				return true;
			});
		}
	}
}


}


