#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/ui.h>

static GtkWidget *host_entry;
static GtkWidget *port_entry;
static GtkWidget *username_entry;
static GtkWidget *pass_entry;
static GtkWidget *button_go;
static GtkWidget *label;

gboolean on_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    (void)widget;
    (void)data;

    if (event->keyval == GDK_KEY_Escape)
        gtk_main_quit();
    return FALSE;
}

void apply_style(GtkCssProvider *provider)
{
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(button_go);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_style_context_save(context);
    //    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    //                                              GTK_STYLE_PROVIDER(provider),TK_STYLE_PROVIDER_PRIORITY_USER);
}

void login_window_init(t_env *e)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_provider_load_css(provider, e->argv_0, "/ui/login.css");

    builder = gtk_builder_new();

    if (gtk_builder_load(builder, e->argv_0, "/ui/login.glade") < 0)
        return;

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_keypress), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    host_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_host"));
    port_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_port"));
    username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
    pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_pass"));

    label = GTK_WIDGET(gtk_builder_get_object(builder, "label_state"));

    button_go = GTK_WIDGET(gtk_builder_get_object(builder, "button_go"));
    g_signal_connect(button_go, "clicked", G_CALLBACK(login_connect), e);

    apply_style(provider);
}

void login_connect(GtkWidget *widget, gpointer data)
{
    t_env *     e;
    char        hostname[NI_MAXHOST + 1];
    const char *host_data;
    const char *port_data;
    const char *username_data;
    const char *pass_data;

    (void)widget;
    e = (t_env *)data;

    gtk_label_set_text(GTK_LABEL(label), "Connecting ...");

    host_data = gtk_entry_get_text(GTK_ENTRY(host_entry));
    port_data = gtk_entry_get_text(GTK_ENTRY(port_entry));
    username_data = gtk_entry_get_text(GTK_ENTRY(username_entry));
    pass_data = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    if (ato32(port_data ? port_data : "5555", (uint32_t *)&e->options.port) !=
            0 ||
        e->options.port < 1000 || e->options.port > 99999)
    {
        logerror("port must be a vakue between 1000 an 99999'%s'.\n",
                 port_data ? port_data : "5555");
        return;
    }
    memcpy(e->options.host, host_data, strlen(host_data));

    if (gethostname(hostname, sizeof(hostname)) == -1)
        return;

    _c2s_pass(e, pass_data, strlen(pass_data));

    _c2s_connect(e, username_data, hostname, host_data);

    if (e->sock != -1)
    {
        gtk_widget_hide(window);
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(pass_entry), "");

        // mainWindow();

        gtk_label_set_text(GTK_LABEL(label), "[!] Failed to connect to server");
    }
}
