namespace AdvvChart {


public abstract class SeriesRenderer : Drawable, Object
{
	private const int VIRTUAL_LEFT_PADDING = -200;

	public bool visible { get; set; default = true; }

	public Path line { get; set; }

	protected SeriesRenderer()
	{
		line = new Path(1);
	}

	protected BoundingBox _bounding_box = BoundingBox() {x=0, y=0, width=0, height=0};
	protected Values _values;
	public Values get_values() { return _values; }

	public abstract void draw(Cairo.Context ctx, Config config);

	public BoundingBox get_bounding_box() { return _bounding_box; }

	protected void debug(Cairo.Context ctx)
	{
		var debug = Environment.get_variable("LIVE_CHART_DEBUG");
		if(debug != null)
		{
			ctx.rectangle(_bounding_box.x, _bounding_box.y, _bounding_box.width, _bounding_box.height);
			ctx.stroke();
		}
	}

	protected bool is_out_of_area(Point point)
	{
		return point.x < SeriesRenderer.VIRTUAL_LEFT_PADDING;
	}
}


}


