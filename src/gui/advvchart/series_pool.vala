namespace AdvvChart { 


public class SeriesPool : Object
{
	private Gee.Map<Series, ulong> _signals = new Gee.HashMap<Series, ulong>();
	private Gee.ArrayList<Series> _series = new Gee.ArrayList<Series>();
	private weak Chart _chart;

	public SeriesPool(Chart chart)
	{
		_chart = chart;
	}

	public Series register(Series series)
	{
		if(_series.contains(series)){
			return series;
		}

		_series.add(series);
		//if values were added to series before registration
		series.get_values().foreach((value) => {_chart.config.y_axis.update_bounds(value.value); return true;});
		
		if(_chart.legend != null) _chart.legend.add_legend(series);
		var sh = series.value_added.connect((value) => {
			_chart.config.y_axis.update_bounds(value);
		});
		_signals[series] = sh;
		return series;
	}

	public new Series get(int index) throws ChartError {
		if (index > _series.size - 1)
		{
			throw new ChartError.SERIE_NOT_FOUND("Serie at index %d not found".printf(index));
		}
		return _series.get(index);
	}

	public Series get_by_name(string name) throws ChartError
	{
		foreach (Series series in _series)
		{
			if (series.name == name) return series;
		}
		throw new ChartError.SERIE_NOT_FOUND("Serie with name %s not found".printf(name));
	}

	public void remove_series(Series series){
		if(_signals.has_key(series))
		{
			var sh = _signals[series];
			series.disconnect(sh);
			_signals.unset(series);
		}
		
		if(_series.contains(series))
		{
			_series.remove(series);
		}
		if(_chart.legend != null)
		{
			_chart.legend.remove_legend(series);
		}
	}

	public void remove_all()
	{
		foreach (var entry in _signals)
		{
			entry.key.disconnect(entry.value);
		}
		_signals.clear();
		_series.clear();
		if(_chart.legend != null)
		{
			_chart.legend.remove_all_legend();
		}
	}

	public Gee.Iterator<Series> iterator()
	{
		return _series.list_iterator();
	}
}


}


