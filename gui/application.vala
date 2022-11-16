/* application.vala
 *
 * Copyright 2022 Oleg Aushev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


namespace CanMonitor {


public class Application : Adw.Application
{
	public Application()
	{
		Object (application_id: "org.example.uCAN-Monitor", flags: ApplicationFlags.FLAGS_NONE);
	}

	construct
	{
		ActionEntry[] action_entries = {
			{"about", this.on_about_action},
			{"preferences", this.on_preferences_action},
			{"canbus", this.on_canbus_action},
			{"quit", this.quit}
		};
		this.add_action_entries (action_entries, this);
		this.set_accels_for_action ("app.quit", {"<primary>q"});

		this.shutdown.connect(main_exit);
        }

	public override void activate()
	{
		main_enter();
		while (!g_isBackendReady) { /* wait */ }
		ucanopen_client_set_tpdo_enabled(WindowCanBusPrefs.switchTpdoState);
		srmdrive_observer_set_watch_enabled(WindowCanBusPrefs.switchWatchState);
		srmdrive_observer_set_watch_period(WindowCanBusPrefs.watchPeriodDefault);

		base.activate();
		var win = this.active_window;
		if (win == null)
		{
			win = new CanMonitor.Window (this);
		}
		win.present();
        }

	private void on_about_action()
	{
		string[] authors = { "Oleg Aushev" };
		Gtk.show_about_dialog (this.active_window,
				"program-name", "uCAN Monitor",
				"authors", authors,
				"version", GIT_DESCRIBE);
	}

	private void on_preferences_action()
	{
		message ("app.preferences action activated");
	}

	private void on_canbus_action()
	{
		var win_canbus = new CanMonitor.WindowCanBusPrefs();
		win_canbus.present();
	}
}


}

