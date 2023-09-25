namespace AdvvChart { 

public class Font {
	public uint8 size { get; set; }
	public string family { get; set; }
	public Cairo.FontSlant slant { get; set; }
	public Cairo.FontWeight weight { get; set; }
	public Gdk.RGBA color { get; set; }

	public Font()
	{
		size = 10;
		family = "Sans serif";
		slant = Cairo.FontSlant.NORMAL;
		weight = Cairo.FontWeight.NORMAL;
		color = {0.4f, 0.4f, 0.4f, 1.0f};
	}

	public void configure(Cairo.Context ctx)
	{
		ctx.select_font_face(family, slant, weight);
		ctx.set_font_size(size);
		ctx.set_source_rgba(color.red, color.green, color.blue, color.alpha);
	}
}


}