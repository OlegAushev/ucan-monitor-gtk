/**
 * @file ucan_monitor_window.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "ucan_monitor_window.h"


G_DEFINE_TYPE (UcanMonitorWindow, ucan_monitor_window, GTK_TYPE_APPLICATION_WINDOW)

static void ucan_monitor_window_class_init (UcanMonitorWindowClass *klass)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);

	gtk_widget_class_set_template_from_resource(widget_class, "ucan_monitor_window.ui");
	gtk_widget_class_bind_template_child(widget_class, UcanMonitorWindow, header_bar);
	gtk_widget_class_bind_template_child(widget_class, UcanMonitorWindow, label);
}

static void ucan_monitor_window_init (UcanMonitorWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}
