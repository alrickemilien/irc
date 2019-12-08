#include <client/ui/panel.h>

int ui_handle_i18n_direction(GtkWidget *label, GtkWidget *hour, const char *msg)
{
#ifdef __APPLE__
    const int is_rtl =
        (gtk_pango_find_base_dir(msg, strlen(msg)) == PANGO_DIRECTION_RTL);
#else
    const int is_rtl =
        (pango_find_base_dir(msg, strlen(msg)) == PANGO_DIRECTION_RTL);
#endif

    if (is_rtl)
    {
        gtk_widget_set_direction(label, GTK_TEXT_DIR_RTL);
        gtk_label_set_xalign(GTK_LABEL(label), 0);
        gtk_label_set_xalign(GTK_LABEL(hour), 1);
        return (GTK_TEXT_DIR_RTL);
    }
    
    gtk_widget_set_direction(label, GTK_TEXT_DIR_LTR);
    gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_label_set_xalign(GTK_LABEL(hour), 0);

    return (GTK_TEXT_DIR_LTR);
}