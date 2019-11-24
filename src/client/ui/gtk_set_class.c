#include <client/ui/ui.h>

void gtk_set_class(GtkWidget *w, const char *class)
{
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(w);
    gtk_style_context_add_class(context, class);
}