namespace AdvvChart {


public struct BoundingBox
{
	double x;
	double y;
	double width;
	double height;
}

public interface Drawable : Object
{
	public abstract bool visible { get; set; default = true; }
	public abstract void draw(Cairo.Context ctx, Config config);
	public abstract BoundingBox get_bounding_box();
}


}


