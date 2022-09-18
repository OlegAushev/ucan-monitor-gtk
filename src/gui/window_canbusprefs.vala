///
///
///


extern void cansocket_connect(string interface, int bitrate);
extern void cansocket_disconnect();


namespace CanMonitor {


[GtkTemplate (ui = "/src/gui/window_canbusprefs.ui")]
public class WindowCanBusPrefs : Adw.PreferencesWindow
{
	[GtkChild]
	private unowned Gtk.Button buttonConnect; 

	[GtkChild]
	private unowned Gtk.Button buttonDisconnect;

	[GtkChild]
	private unowned Adw.ComboRow comborowCanInterface;

	[GtkChild]
	private unowned Adw.ComboRow comborowCanBitrate;

	[GtkChild]
	private unowned Gtk.StringList listCanInterface;

	[GtkChild]
	private unowned Gtk.StringList listCanBitrate;
	
	public WindowCanBusPrefs()
	{
		buttonConnect.clicked.connect(() => {
			cansocket_connect(listCanInterface.get_string(comborowCanInterface.get_selected())
					, int.parse(listCanBitrate.get_string(comborowCanBitrate.get_selected())));
		});
		buttonDisconnect.clicked.connect(cansocket_disconnect);
	}
}


}

    
