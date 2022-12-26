namespace AdvvChart {


public class Background : Object
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
			red = 1.0f,
			green = 0.0f,
			blue = 0.0f,
			alpha = 1.0f
		};
	}

	public void draw(Cairo.Context ctx, Config config)
	{
		if (visible)
		{
			update_bounding_box(config);
			ctx.rectangle(0, 0, config.width, config.height);
			ctx.set_source_rgba(color.red, color.green, color.blue, color.alpha);
			ctx.fill();
		}
	}

	private void update_bounding_box(Config config)
	{
		bounding_box = BoundingBox() {
			x=0, 
			y=0, 
			width=config.width,
			height=config.height
		};
	}
}


}


