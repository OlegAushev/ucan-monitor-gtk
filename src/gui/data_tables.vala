///
///
///





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
	
	int msec = 0;

	public bool update()
	{
		msec += 50;
		entryUptime.entry_text = msec.to_string();
		return true;
	}
}


