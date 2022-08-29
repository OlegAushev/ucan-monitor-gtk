/**
 * @file monitor_window.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <gtk/gtk.h>


G_BEGIN_DECLS


#define MONITOR_TYPE_WINDOW (monitor_window_get_type())

G_DECLARE_FINAL_TYPE(MonitorWindow, monitor_window, MONITOR, WINDOW, GtkApplicationWindow)


struct _MonitorWindow
{
	GtkApplicationWindow  parent_instance;

	/* Template widgets */
	GtkHeaderBar        *header_bar;
	GtkLabel            *label;
};


G_END_DECLS

