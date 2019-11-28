#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/login.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static gboolean on_keypress(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data)
{
    (void)widget;

    if (event->keyval == GDK_KEY_Escape)
    {
        ui_clear_login_window(data);
        gtk_main_quit();
    }
    return FALSE;
}

int ui_init_login_window(t_env *e, t_ui_login *ui)
{
    GtkCssProvider *css_provider;

    ui->e = e;

    ui->builder = gtk_builder_new();
    if (gtk_builder_load(ui->builder, e->argv_0, "/ui/login/login.glade") < 0)
        return (-1);

    // Init window and close events
    ui->window =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "window_login"));

    gtk_widget_add_events(ui->window, GDK_KEY_PRESS_MASK);
    g_signal_connect(ui->window, "key_press_event", G_CALLBACK(on_keypress),
                     ui);
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Set entrys
    ui->host_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_host"));
    ui->port_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_port"));
    ui->username_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_name"));
    ui->pass_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_pass"));
    ui->nick_entry =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "entry_nick"));

    // Set login button event
    ui->button_go =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "button_go"));
    g_signal_connect(ui->button_go, "clicked", G_CALLBACK(ui_login_connect),
                     ui);

    // Apply style to window
    css_provider = gtk_css_provider_new();
    if (gtk_provider_load_css(css_provider, e->argv_0, "/ui/login/login.css") <
        0)
        return (-1);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    ui->window_color = gtk_new_rgba(1, 1, 1, 0.96);

    gtk_set_transparent_window(ui->window, ui->window_color);

    g_object_unref(G_OBJECT(css_provider));

    gtk_widget_show_all(ui->window);

    return (0);
}

int ui_clear_login_window(t_ui_login *ui)
{
    free(ui->window_color);
    g_object_unref(G_OBJECT(ui->builder));
    return (0);
}

void ui_login_connect(GtkWidget *widget, gpointer data)
{
    t_ui_login *ui;
    t_env *     e;
    const char *host_data;
    const char *port_data;
    const char *username_data;
    const char *pass_data;
    const char *nick_data;

    (void)widget;
    ui = (t_ui_login *)data;
    e = ui->e;

    gtk_button_set_label(GTK_BUTTON(ui->button_go), "Connecting ...");

    host_data = gtk_entry_get_text(GTK_ENTRY(ui->host_entry));
    port_data = gtk_entry_get_text(GTK_ENTRY(ui->port_entry));
    username_data = gtk_entry_get_text(GTK_ENTRY(ui->username_entry));
    pass_data = gtk_entry_get_text(GTK_ENTRY(ui->pass_entry));
    nick_data = gtk_entry_get_text(GTK_ENTRY(ui->nick_entry));

    if (ato32(port_data[0] ? port_data : "5555",
              (uint32_t *)&e->options.port) != 0 ||
        e->options.port < 1000 || e->options.port > 99999)
    {
        logerror("port must be a vakue between 1000 an 99999'%s'.\n",
                 port_data ? port_data : "5555");
        return;
    }

    logdebug("e->options.host: %s\n", e->options.host);
    logdebug("host_data: %s\n", host_data);

    memcpy(e->options.host, host_data && host_data[0] ? host_data : "127.0.0.1",
           host_data && host_data[0] ? strlen(host_data) : strlen("127.0.0.1"));

    _c2s_pass(e, pass_data, strlen(pass_data));

    _c2s_nick(e, nick_data, strlen(nick_data));

    _c2s_connect(e, username_data[0] ? username_data : NULL, NULL,
                 e->options.host);

    if (e->sock != -1)
    {
        // gtk_widget_hide(ui->window);

        // Load loop select
        g_idle_add((GSourceFunc)gtk_do_select, e);

        // panel_login = panel_window(e);

        // gtk_widget_show_all(panel_login);
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(ui->pass_entry), "");

        gtk_button_set_label(GTK_BUTTON(ui->button_go), "Log in");

        // gtk_label_set_text(GTK_LABEL(label), "[!] Failed to connect to
        // server");
    }
}
