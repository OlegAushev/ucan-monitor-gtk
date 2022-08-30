/**
 * @file monitor_headerbar.h
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


#define MONITOR_TYPE_HEADERBAR (monitor_headerbar_get_type())

GtkWidget* monitor_headerbar_new();


G_END_DECLS


