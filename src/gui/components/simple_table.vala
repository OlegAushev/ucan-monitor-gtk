///
///
///


[GtkTemplate (ui = "/src/gui/components/simple_table.ui")]
public class SimpleTable : Adw.Bin
{
	[GtkChild]
	private unowned Adw.PreferencesGroup _group;

	public SimpleTable()
	{

		
	}

	construct
	{
		SimpleTableRow label1 = new SimpleTableRow();
		SimpleTableRow label2 = new SimpleTableRow();
		SimpleTableRow label3 = new SimpleTableRow();
		SimpleTableRow label4 = new SimpleTableRow();
		SimpleTableRow label5 = new SimpleTableRow();
		_group.add(label1);
		_group.add(label2);
		_group.add(label3);
		_group.add(label4);	
		_group.add(label5);

		label1.title = "Uptime";
		label1.subtitle = "Time since system start";
		label1.entry_text = "12345.67"; 
		label1.entry_width = 100;
		label1.entry_chars = 10;
	}
}


