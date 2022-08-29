/**
 * @file ucan_monitor_application.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include <adwaita.h>


G_BEGIN_DECLS


#define UCAN_MONITOR_TYPE_APPLICATION (ucan_monitor_application_get_type())

G_DECLARE_FINAL_TYPE(UcanMonitorApplication, ucan_monitor_application, UCAN_MONITOR, APPLICATION, AdwApplication)

UcanMonitorApplication* ucan_monitor_application_new(const char* application_id,
                                                    GApplicationFlags flags);

struct _UcanMonitorApplication
{
	GtkApplication parent_instance;
};


G_END_DECLS


