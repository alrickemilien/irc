#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>

static GtkWidget *host_entry;
static GtkWidget *port_entry;
static GtkWidget *username_entry;
static GtkWidget *pass_entry;
static GtkWidget *button_go;
static GtkWidget *window_login;
static GtkWidget *panel_login;

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

GtkWidget *login_window(t_env *e)
{
    GtkBuilder *    builder;
    GtkCssProvider *cssProvider;

    builder = gtk_builder_new();
    if (gtk_builder_load(builder, e->argv_0, "/ui/login/login.glade") < 0)
        return ((void*)0);

    cssProvider = gtk_css_provider_new();
    if (gtk_provider_load_css(cssProvider, e->argv_0, "/ui/login/login.css") <
        0)
        return ((void*)0);

    window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    gtk_widget_add_events(window_login, GDK_KEY_PRESS_MASK);
    g_signal_connect(window_login, "key_press_event", G_CALLBACK(on_keypress), NULL);
    g_signal_connect(window_login, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    host_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_host"));
    port_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_port"));
    username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
    pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_pass"));

    button_go = GTK_WIDGET(gtk_builder_get_object(builder, "button_go"));
    g_signal_connect(button_go, "clicked", G_CALLBACK(login_connect), e);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(G_OBJECT(builder));
    g_object_unref(G_OBJECT(cssProvider));

    return (window_login);
}

void login_connect(GtkWidget *widget, gpointer data)
{
    t_env *     e;
    const char *host_data;
    const char *port_data;
    const char *username_data;
    const char *pass_data;

    (void)widget;
    e = (t_env *)data;

    gtk_button_set_label(GTK_BUTTON(button_go), "Connecting ...");

    host_data = gtk_entry_get_text(GTK_ENTRY(host_entry));
    port_data = gtk_entry_get_text(GTK_ENTRY(port_entry));
    username_data = gtk_entry_get_text(GTK_ENTRY(username_entry));
    pass_data = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    if (ato32(port_data[0] ? port_data : "5555",
              (uint32_t *)&e->options.port) != 0 ||
        e->options.port < 1000 || e->options.port > 99999)
    {
        logerror("port must be a vakue between 1000 an 99999'%s'.\n",
                 port_data ? port_data : "5555");
        return;
    }

    memcpy(e->options.host, host_data[0] ? host_data : "127.0.0.1",
           host_data[0] ? strlen(host_data) : strlen("127.0.0.1"));

    _c2s_pass(e, pass_data, strlen(pass_data));

    _c2s_connect(e, username_data, NULL, e->options.host);

    if (e->sock != -1)
    {
        gtk_widget_hide(window_login);

        panel_login = panel_window(e);

    	gtk_widget_show_all(panel_login);
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(pass_entry), "");

        gtk_button_set_label(GTK_BUTTON(button_go), "Log in");

        // gtk_label_set_text(GTK_LABEL(label), "[!] Failed to connect to
        // server");
    }
}
