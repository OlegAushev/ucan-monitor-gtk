#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define APP_TYPE_WINDOW (app_window_get_type())

G_DECLARE_FINAL_TYPE (AppWindow, app_window, APP, WINDOW, GtkApplicationWindow)

G_END_DECLS