namespace AdvvChart {
   
	
public abstract class Legend : Drawable, Object
{
	public bool visible { get; set; default = true; }
	public Labels labels = new Labels();

	protected Gee.ArrayList<Series> _series = new Gee.ArrayList<Series>();
	protected BoundingBox _bounding_box = BoundingBox() {x=0, y=0, width=0, height=0};
	
	public Gdk.RGBA main_color
	{ 
		get; set; default= Gdk.RGBA() {red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f};
	}

	public void add_legend(Series series) { _series.add(series); }
	
	public void remove_legend(Series series)
	{
		if (_series.contains(series))
		{
			_series.remove(series);
		}
	}
	
	public void remove_all_legend()
	{
		_series.clear();
	}
	
	public abstract void draw(Cairo.Context ctx, Config config);
	public BoundingBox get_bounding_box() { return _bounding_box; }
}

public class HorizontalLegend : Legend {
	
	private const int COLOR_BLOCK_WIDTH = 15;
	private const int COLOR_BLOCK_HEIGHT = 10;

	public override void draw(Cairo.Context ctx, Config config)
	{
		if (!visible) return;
		
		var y_padding = get_y_padding(config);
		var boundaries = config.boundaries();
		var pos = 0;
		_series.foreach((series) => {
			ctx.set_source_rgba(series.line.color.red, series.line.color.green, series.line.color.blue, 1);
			ctx.rectangle(boundaries.x.min + pos, boundaries.y.max + y_padding, HorizontalLegend.COLOR_BLOCK_WIDTH, HorizontalLegend.COLOR_BLOCK_HEIGHT);
			ctx.fill();
			
			labels.font.configure(ctx);
			Cairo.TextExtents extents = name_extents(series.name, ctx);
			ctx.move_to(boundaries.x.min + pos + HorizontalLegend.COLOR_BLOCK_WIDTH + 3, boundaries.y.max + y_padding + extents.height + (HorizontalLegend.COLOR_BLOCK_HEIGHT - extents.height) / 2);
			ctx.show_text(series.name);

			pos += HorizontalLegend.COLOR_BLOCK_WIDTH + (int) extents.width + 20;

			return true;
		});
		ctx.stroke();
		this.update_bounding_box(config, pos);
		this.debug(ctx);
	}

	private int get_y_padding(Config config)
	{
		return (int)(Grid.ABSCISSA_TIME_PADDING * 2 + config.x_axis.labels.extents.height);
	}

	private Cairo.TextExtents name_extents(string name, Cairo.Context ctx)
	{
		Cairo.TextExtents name_extents;
		ctx.text_extents(name, out name_extents);
		return name_extents;
	}

	private void update_bounding_box(Config config, int width) 
	{
		var boundaries = config.boundaries();
		_bounding_box = BoundingBox() {
			x=boundaries.x.min,
			y=boundaries.y.max + get_y_padding(config),
			width=width,
			height=10
		};
	}

	protected void debug(Cairo.Context ctx) {
		var debug = Environment.get_variable("LIVE_CHART_DEBUG");
		if(debug != null)
		{
			ctx.rectangle(_bounding_box.x, _bounding_box.y, _bounding_box.width, _bounding_box.height);
			ctx.stroke();
		}
	}        
}

public class NoopLegend : Legend 
{
	public override void draw(Cairo.Context ctx, Config config) {}
}


}


