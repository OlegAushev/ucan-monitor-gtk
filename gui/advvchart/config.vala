namespace AdvvChart {


[Flags]
public enum AutoPadding {
	NONE,
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
}


public struct Padding
{
	public AutoPadding smart;
	public int top;
	public int right;
	public int bottom;
	public int left;

	public Padding()
	{
		smart = AutoPadding.TOP | AutoPadding.RIGHT | AutoPadding.BOTTOM | AutoPadding.LEFT;
		top = 20;
		right = 20;
		bottom = 20;
		left = 20;
	}
}


public struct Boundary
{
	public int min;
	public int max;
}


public struct Boundaries
{
	public Boundary x;
	public Boundary y;
	public int width;
	public int height;
}


public class Config
{
	public int width { get; set; default = 0; }
	public int height { get; set; default = 0; }

	public Padding padding = Padding();

	public XAxis x_axis = new XAxis();
	public YAxis y_axis = new YAxis();

	public TimeSeek time = new TimeSeek();

	public Boundaries boundaries() {
		return Boundaries() {
			x = {min: padding.left, max: width - padding.right},
			y = {min: padding.top, max: height - padding.bottom},
			width =  width - padding.right - padding.left,
			height = height - padding.bottom - padding.top
		};
	}
}


}


