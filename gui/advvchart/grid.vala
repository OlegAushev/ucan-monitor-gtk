namespace AdvvChart {


public class Grid : Drawable, Object
{
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






}


}


