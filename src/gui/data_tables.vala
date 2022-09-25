///
///
///


extern void motordrive_data_get_watch(string name, string value);


[GtkTemplate (ui = "/src/gui/data_tables.ui")]
public class DataTables : Adw.Bin
{
	[GtkChild]
	private unowned TableEntry entryUptime;

	public DataTables()
	{

		
	}

	construct
	{
		Timeout.add(50, update);
	}
	
	public bool update()
	{
		string result = string.nfill(16, '\0');
		motordrive_data_get_watch("UPTIME", result);
		entryUptime.entry_text = result;
		return true;
	}
}


