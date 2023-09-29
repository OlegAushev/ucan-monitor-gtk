namespace AdvvChart {


public class Line : SeriesRenderer {

	public Line(Values values = new Values())
	{
		base();
		_values = values;
	}

	public override void draw(Cairo.Context ctx, Config config)
	{
		if (!visible) return;

		var points = Points.create(_values, config);
		if (points.size > 0)
		{
			draw_line(points, ctx, config);
			ctx.stroke();
		}
	}

	protected Points draw_line(Points points, Cairo.Context ctx, Config config)
	{
		line.configure(ctx);
		
		var first_point = points.first();

		this.update_bounding_box(points, config);
		this.debug(ctx);

		line.configure(ctx);

		ctx.move_to(first_point.x, first_point.y);
		for (int pos = 0; pos < points.size -1; pos++)
		{
			var current_point = points.get(pos);
			var next_point = points.after(pos);
			if (is_out_of_area(current_point))
			{
				ctx.move_to(current_point.x, current_point.y);
				continue;
			}

			ctx.line_to(next_point.x, next_point.y);
		}
	
		return points;
	}

	private void update_bounding_box(Points points, Config config)
	{
		_bounding_box = BoundingBox() {
			x=points.first().x,
			y=points.bounds.lower,
			width=points.last().x - points.first().x,
			height=points.bounds.upper - points.bounds.lower
		};
	}
}


}


