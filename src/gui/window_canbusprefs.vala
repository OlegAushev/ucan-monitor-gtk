///
///
///


extern void cansocket_connect();
extern void cansocket_disconnect();


namespace CanMonitor {


[GtkTemplate (ui = "/src/gui/window_canbusprefs.ui")]
public class WindowCanBusPrefs : Adw.PreferencesWindow
{
	[GtkChild]
	private unowned Gtk.Button buttonConnect; 

	[GtkChild]
	private unowned Gtk.Button buttonDisconnect;
	
	public WindowCanBusPrefs()
	{
		buttonConnect.clicked.connect(cansocket_connect);
		buttonDisconnect.clicked.connect(cansocket_disconnect);
	}
}


}

    
