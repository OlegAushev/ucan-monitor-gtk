namespace AdvvChart {


public class Grid : Drawable, Object
{
	public const int ABSCISSA_TIME_PADDING = 5;

        private BoundingBox bounding_box = BoundingBox() {
		x=0, 
		y=0, 
		width=0,
		height=0
	};

	public bool visible { get; set; default = true; }

	public Gdk.RGBA color
	{
		get; set; default = Gdk.RGBA() {
			red = 0.4f,
			green = 0.4f,
			blue = 0.4f,
			alpha = 1.0f
		};
	}

	public void draw(Cairo.Context ctx, Config config)
	{
		if (!visible) return;
		draw_x_axis(ctx, config);
		draw_y_axis(ctx, config);
		draw_grid(ctx, config);
	}

	public BoundingBox get_bounding_box() {	return bounding_box; }

	protected void restore(Cairo.Context ctx) {
		ctx.set_source_rgba(color.red, color.green, color.blue, color.alpha);
		ctx.set_line_width(0.5);
		ctx.set_dash(null, 0.0);
	}

	protected void draw_x_axis(Cairo.Context ctx, Config config)
	{
		if (!config.x_axis.visible || !config.x_axis.axis.visible) return;
		
		config.x_axis.axis.configure(ctx);
		ctx.move_to(config.padding.left + 0.5, config.height - config.padding.bottom + 0.5);
		ctx.line_to(config.width - config.padding.right + 0.5, config.height - config.padding.bottom + 0.5);
		ctx.stroke();
		restore(ctx);
	}

	protected void draw_y_axis(Cairo.Context ctx, Config config)
	{
		if (!config.y_axis.visible || !config.y_axis.axis.visible) return;
		
		config.y_axis.axis.configure(ctx);
		ctx.move_to(config.padding.left + 0.5, config.height - config.padding.bottom + 0.5);
		ctx.line_to(config.padding.left + 0.5, config.padding.top + 0.5);
		ctx.stroke();
		restore(ctx);
	}

	protected void draw_grid(Cairo.Context ctx, Config config)
	{
		draw_v_grid(ctx, config);
		draw_h_grid(ctx, config);
	}

	protected void draw_v_grid(Cairo.Context ctx, Config config)
	{
		var grid_interval = (int64)(config.x_axis.tick_interval * config.time.conv_sec);
		var time = config.time.current;
		for (double i = config.width - config.padding.right; i > config.padding.left; i -= config.x_axis.tick_length)
		{
			if (config.x_axis.lines.visible)
			{
				config.x_axis.lines.configure(ctx);
				ctx.move_to((int)i + 0.5, 0.5 + config.height - config.padding.bottom);
				ctx.line_to((int)i + 0.5, 0.5 + config.padding.top);
				ctx.stroke();
				restore(ctx);
			}
			
			// Labels
			if (config.x_axis.visible && config.x_axis.labels.visible)
			{
				config.x_axis.labels.font.configure(ctx);
				var text = config.time.get_time_str(time, config.x_axis.show_fraction);
				Cairo.TextExtents extents;
				ctx.text_extents(text, out extents);
				
				ctx.move_to(i + 0.5 - extents.width / 2, 0.5 + config.height - config.padding.bottom + config.x_axis.labels.extents.height + Grid.ABSCISSA_TIME_PADDING);
				ctx.show_text(text);
				ctx.stroke();
			}
			time -= grid_interval;
		}
	}

	protected void draw_h_grid(Cairo.Context ctx, Config config)
	{
		var boundaries = config.boundaries();
		foreach(float position in config.y_axis.ticks.values)
		{

			var y = boundaries.height + boundaries.y.min - position * config.y_axis.get_ratio();
			if(y < boundaries.y.min)
			{
				break;
			}
			if (config.y_axis.lines.visible)
			{
				config.y_axis.lines.configure(ctx);
				ctx.move_to(0.5 + boundaries.x.max, (int)y + 0.5);
				ctx.line_to(boundaries.x.min + 0.5, (int)y  + 0.5);
				ctx.stroke();
				restore(ctx);
			}

			//Labels
			if (config.y_axis.visible && config.y_axis.labels.visible)
			{
				config.y_axis.labels.font.configure(ctx);
				var value = format_for_y_axis(config.y_axis.unit, position);
		
				Cairo.TextExtents extents;
				ctx.text_extents(value, out extents);
				ctx.move_to(boundaries.x.min - extents.width - 5, y + (extents.height / 2) + 0.5);
				ctx.show_text(value);
				ctx.stroke();
			}
		}
	}

	protected void update_bounding_box(Config config)
	{
		var boundaries = config.boundaries();
		this.bounding_box = BoundingBox() {
			x=boundaries.x.min,
			y=boundaries.y.min,
			width=boundaries.x.max - boundaries.x.min, 
			height=boundaries.y.max - boundaries.y.min + Grid.ABSCISSA_TIME_PADDING
		};
	}

}


}


