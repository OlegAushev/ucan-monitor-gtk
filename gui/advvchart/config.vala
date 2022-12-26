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



public class Config
{
	public int width { get; set; default = 0; }
	public int height { get; set; default = 0; }

	public Padding padding = Padding();

	public XAxis x_axis = new XAxis();
	public YAxis y_axis = new YAxis();
}


}


