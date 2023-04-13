///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_heartbeatindicator.ui")]
public class HeartbeatIndicator : BasicEntry
{
	public HeartbeatIndicator() {}

	construct {}

	public void update()
	{
		if (!ucanopen_server_is_heartbeat_ok(Backend.Ucanopen.server))
		{
			entry_remove_css_class("success");
			string_value = "no HB";
			entry_add_css_class("error");
		}
		else
		{
			entry_remove_css_class("error");
			string nmt_state = string.nfill(16, '\0');
			ucanopen_server_get_nmt_state(Backend.Ucanopen.server, nmt_state, 16);
			string_value = nmt_state;
			if (nmt_state == "run")
			{
				entry_add_css_class("success");
			}
			else
			{
				entry_add_css_class("warning");
			}
		}
	}

}


}


