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


//#include <iostream>
//#include <memory>
#include <adwaita.h>

/*
void onAppActivate(GtkApplication* app)
{
	GtkWidget* window = gtk_application_window_new(app);
	GtkWidget* label = gtk_label_new("Hello GTK");

	gtk_window_set_title(GTK_WINDOW (window), "UCAN monitor");
	gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);
	gtk_window_set_child(GTK_WINDOW (window), label);
	gtk_window_present(GTK_WINDOW (window));
}


int main(int argc, char* argv[])
{
	std::unique_ptr<AdwApplication> app(adw_application_new("org.example.ucan-monitor", G_APPLICATION_FLAGS_NONE));
	g_signal_connect(app.get(), "activate", G_CALLBACK(onAppActivate), NULL);
	return g_application_run (G_APPLICATION(app.get()), argc, argv);
}*/

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window, *label;

  adw_init ();

  window = gtk_window_new ();
  label = gtk_label_new ("Hello World");

  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_window_set_child (GTK_WINDOW (window), label);
  gtk_window_present (GTK_WINDOW (window));

   while (g_list_model_get_n_items (gtk_window_get_toplevels ()) > 0)
    g_main_context_iteration (NULL, TRUE);

  return 0;
}