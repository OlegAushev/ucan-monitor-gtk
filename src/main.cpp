/**
 * @file main.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include <memory>
#include <adwaita.h>

#include <glib/gi18n.h>
#include "gui/monitor_application.h"


int main(int argc, char* argv[])
{
	MonitorApplication* app = monitor_application_new("com.example.uCAN-Monitor",
			G_APPLICATION_FLAGS_NONE);

	return g_application_run(G_APPLICATION(app), argc, argv);
}
