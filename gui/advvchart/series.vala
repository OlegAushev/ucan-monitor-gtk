namespace AdvvChart {


public class Series : Drawable, Object
{
	public string name { get; set; }
	
	public Path line
	{
		get { return _renderer.line; }
		set { _renderer.line = value; }
	}

	public bool visible { get; set; default = true; }

	public signal void value_added(double value);

	private SeriesRenderer _renderer;

	public Series(string name_, SeriesRenderer renderer_ = new Line()) {
		name = name_;
		_renderer = renderer_;
	}

	public void draw(Cairo.Context ctx, Config config)
	{
		if (visible)
		{
			_renderer.draw(ctx, config);
		}
	}

	public void add(double value)
	{
		_renderer.get_values().add({GLib.get_real_time() / 1000, value});
		value_added(value);
	}

	public void add_with_timestamp(double value, int64 timestamp)
	{
		_renderer.get_values().add({timestamp, value});
		value_added(value);
	}

	public Values get_values() { return _renderer.get_values(); }

	public void clear() { _renderer.get_values().clear(); }

	public BoundingBox get_bounding_box() { return _renderer.get_bounding_box(); }
}


}


