#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <client/irc.h>
#include <client/ui/panel.h>

// static GtkWidget *host_entry;
// static GtkWidget *port_entry;
// static GtkWidget *username_entry;
// static GtkWidget *pass_entry;
// static GtkWidget *button_go;
// static GtkWidget *label;
static GtkWidget *window_panel;

static gboolean on_keypress(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data)
{
    (void)widget;
    (void)data;

    if (event->keyval == GDK_KEY_Escape)
        gtk_main_quit();
    return FALSE;
}

GtkWidget *panel_window(t_env *e)
{
    GtkBuilder *    builder;
    GtkCssProvider *cssProvider;

    builder = gtk_builder_new();
    if (gtk_builder_load(builder, e->argv_0, "/ui/panel/panel.glade") < 0)
        return ((void *)0);

    cssProvider = gtk_css_provider_new();
    if (gtk_provider_load_css(cssProvider, e->argv_0, "/ui/panel/panel.css") <
        0)
        return ((void *)0);

    window_panel = GTK_WIDGET(gtk_builder_get_object(builder, "window_panel"));

    gtk_widget_add_events(window_panel, GDK_KEY_PRESS_MASK);
    g_signal_connect(window_panel, "key_press_event", G_CALLBACK(on_keypress),
                     NULL);
    g_signal_connect(window_panel, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // host_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_host"));
    // port_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_port"));
    // username_entry = GTK_WIDGET(gtk_builder_get_object(builder,
    // "entry_name"));
    // pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_pass"));

    // button_go = GTK_WIDGET(gtk_builder_get_object(builder, "button_go"));
    // g_signal_connect(button_go, "clicked", G_CALLBACK(login_connect), e);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(G_OBJECT(builder));
    g_object_unref(G_OBJECT(cssProvider));
    return (window_panel);
}