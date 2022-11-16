///
///
///


namespace AdvvChart {


public class Chart : Gtk.DrawingArea
{
	public Background background { get; set; default = new Background(); }




	public Chart()
	{

	}

	construct
	{
		//this.set_draw_func (owned Gtk.DrawingAreaDrawFunc? draw_func)
	}
}


}


