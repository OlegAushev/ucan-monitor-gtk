/**
 * @file ucan_monitor_application.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucan_monitor_application.h"
#include "ucan_monitor_window.h"


G_DEFINE_TYPE(UcanMonitorApplication, ucan_monitor_application, ADW_TYPE_APPLICATION)


///
///
///
UcanMonitorApplication* ucan_monitor_application_new(const char* application_id,
						GApplicationFlags flags)
{
	return g_object_new(UCAN_MONITOR_TYPE_APPLICATION,
			"application-id", application_id,
			"flags", flags,
			NULL);
}


///
///
///
static void ucan_monitor_application_finalize(GObject* object)
{
	UcanMonitorApplication* self = (UcanMonitorApplication*)object;
	G_OBJECT_CLASS(ucan_monitor_application_parent_class)->finalize(object);
}


///
///
///
static void ucan_monitor_application_activate(GApplication* app)
{
	/* It's good practice to check your parameters at the beginning of the
	* function. It helps catch errors early and in development instead of
	* by your users.
	*/
	g_assert (GTK_IS_APPLICATION(app));

	/* Get the current window or create one if necessary. */
	GtkWindow* window = gtk_application_get_active_window(GTK_APPLICATION(app));
	if (window == NULL)
	{
		window = g_object_new(UCAN_MONITOR_TYPE_WINDOW,
				"application", app,
				NULL);
	}

	/* Ask the window manager/compositor to present the window. */
	gtk_window_present (window);
}


///
///
///
static void ucan_monitor_application_class_init(UcanMonitorApplicationClass* klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS(klass);
	GApplicationClass* app_class = G_APPLICATION_CLASS(klass);

	object_class->finalize = ucan_monitor_application_finalize;

	/*
	* We connect to the activate callback to create a window when the application
	* has been launched. Additionally, this callback notifies us when the user
	* tries to launch a "second instance" of the application. When they try
	* to do that, we'll just present any existing window.
	*/
	app_class->activate = ucan_monitor_application_activate;
}


///
///
///
static void ucan_monitor_application_show_about (GSimpleAction *action,
                                    GVariant      *parameter,
                                    gpointer       user_data)
{
  UcanMonitorApplication* self = UCAN_MONITOR_APPLICATION(user_data);
  GtkWindow *window = NULL;
  const gchar *authors[] = {"Oleg Aushev", NULL};

  g_return_if_fail (UCAN_MONITOR_IS_APPLICATION (self));

  window = gtk_application_get_active_window (GTK_APPLICATION (self));

  gtk_show_about_dialog (window,
                         "program-name", "uCAN Monitor",
                         "authors", authors,
                         "version", "0.1.0",
                         NULL);
}


///
///
///
static void ucan_monitor_application_init(UcanMonitorApplication* self)
{
	g_autoptr(GSimpleAction) quit_action = g_simple_action_new("quit", NULL);
	g_signal_connect_swapped(quit_action, "activate", G_CALLBACK (g_application_quit), self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (quit_action));

	g_autoptr(GSimpleAction) about_action = g_simple_action_new ("about", NULL);
	g_signal_connect(about_action, "activate", G_CALLBACK (ucan_monitor_application_show_about), self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (about_action));

	const char* accels[] = {"<primary>q", NULL};
	gtk_application_set_accels_for_action(GTK_APPLICATION (self),
						"app.quit",
						accels);
}