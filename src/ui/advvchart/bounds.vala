namespace AdvvChart {

public class Bounds : Object {

	public double lower { get; private set;	}
	public double upper { get; private set;	}

	public Bounds(double lower_ = double.NAN, double upper_ = double.NAN)
	{
		lower = lower_;
		upper = upper_;
	}

	public bool has_lower() { return !lower.is_nan(); }

	public bool has_upper() { return !upper.is_nan(); }

	public bool update(double value) {
		var updated = false;
		if (!has_lower())
		{
			lower = value;
			updated = true;
		}

		if (!has_upper())
		{
			upper = value;
			updated = true;             
		}

		if (value < lower)
		{
			lower = value;
			updated = true;             
		}

		if (value > upper)
		{
			upper = value;
			updated = true;
		}
		
		return updated;
	}
}


}


