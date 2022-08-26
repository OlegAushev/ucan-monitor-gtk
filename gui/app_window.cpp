#include "app_window.h"

struct _App1Window
{
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkLabel            *label;
};

G_DEFINE_TYPE (AppWindow, app_window, GTK_TYPE_APPLICATION_WINDOW)

static void
app_window_class_init (AppWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "app_window.ui");
  gtk_widget_class_bind_template_child (widget_class, AppWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, AppWindow, label);
}

static void
app_window_init (AppWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
