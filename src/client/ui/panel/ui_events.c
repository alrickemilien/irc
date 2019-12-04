#include <client/irc.h>
#include <client/ui/panel.h>

gboolean on_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    (void)widget;

    if (event->keyval == GDK_KEY_Escape)
        gtk_main_quit();
    else if (event->keyval == GDK_KEY_l && (event->state & GDK_CONTROL_MASK))
        ui_chat_empty_chat_box(data);
    return FALSE;
}

void on_entry(GtkWidget *widget, gpointer data)
{
    const char *text;
    char *      tmp;
    t_env *     e;

    e = (t_env *)data;
    text = gtk_entry_get_text(GTK_ENTRY(widget));
    tmp = strdup(text);
    c2s(e, tmp);
    free(tmp);
    gtk_entry_set_text(GTK_ENTRY(widget), "");
}
