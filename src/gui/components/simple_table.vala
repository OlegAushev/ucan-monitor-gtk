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
		SimpleTableRow _labels = new SimpleTableRow();
		_group.add(_labels);
		_group.add(_labels);
		_group.add(_labels);
		_group.add(_labels);	
		_group.add(_labels);
		_group.add(_labels);	
		_group.add(_labels);
		_group.add(_labels);			
	}
}


