///
///
///


namespace CanMonitor {


[GtkTemplate (ui = "/gui/widgets/systemstatus.ui")]
public class SystemStatus : Adw.Bin
{
	public SystemStatus() {}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		
		return true;
	}
}


}


