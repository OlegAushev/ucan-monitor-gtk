/**
 * @file monitor_application.h
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


#define MONITOR_TYPE_APPLICATION (monitor_application_get_type())

G_DECLARE_FINAL_TYPE(MonitorApplication, monitor_application, MONITOR, APPLICATION, AdwApplication)

MonitorApplication* monitor_application_new(const char* application_id, GApplicationFlags flags);


G_END_DECLS


