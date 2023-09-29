namespace AdvvChart {


public struct TimestampedValue
{
	public double timestamp;
	public double value;	
}


public class Values : Gee.LinkedList<TimestampedValue?>
{
	public Bounds bounds { get; construct set; }

	private int _buffer_size;

	public Values(int buffer_size = 10000) {
		bounds = new Bounds();
		_buffer_size = buffer_size;
	}

	public new void add(TimestampedValue value) {
		if (size == _buffer_size)
		{
			remove_at(0);
		}
		bounds.update(value.value);
		base.add(value);
	}
}


}


