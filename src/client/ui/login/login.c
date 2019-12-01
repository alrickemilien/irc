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
    t_env *          e;
    t_ui_login *     ui;
    t_ui_credentials crd;

    (void)widget;
    ui = (t_ui_login *)data;
    e = ui->e;

    gtk_button_set_label(GTK_BUTTON(ui->button_go), "Connecting ...");

    crd.host = gtk_entry_get_text(GTK_ENTRY(ui->host_entry));
    crd.port = gtk_entry_get_text(GTK_ENTRY(ui->port_entry));
    crd.username = gtk_entry_get_text(GTK_ENTRY(ui->username_entry));
    crd.pass = gtk_entry_get_text(GTK_ENTRY(ui->pass_entry));
    crd.nick = gtk_entry_get_text(GTK_ENTRY(ui->nick_entry));

    if (!crd.nick || crd.nick[0] == 0)
    {
        logerror("Nickname must be provided\n");
        return;
    }

    if (ato32(crd.port[0] ? crd.port : "5555", (uint32_t *)&e->options.port) !=
            0 ||
        e->options.port < 1000 || e->options.port > 99999)
    {
        logerror("port must be a vakue between 1000 an 99999'%s'.\n",
                 crd.port ? crd.port : "5555");
        return;
    }

    logdebug("e->options.host: %s\n", e->options.host);
    logdebug("host_data: %s\n", crd.host);

    memcpy(e->options.host, crd.host && crd.host[0] ? crd.host : "127.0.0.1",
           crd.host && crd.host[0] ? strlen(crd.host) : strlen("127.0.0.1"));

    // printf("pass_data: %s\n", crd.pass);
    // printf("strtrim(pass_data): %s\n", strtrim(crd.pass));
    // printf("len(pass_data): %ld\n", strlentrim(crd.pass));
    // printf("nick_data: %s\n", crd.nick);
    // printf("strtrim(nick_data): %s\n", strtrim(crd.nick));
    // printf("len(nick_data): %ld\n", strlentrim(crd.nick));

    _c2s_pass(e, strtrim(crd.pass), strlentrim(crd.pass));

    _c2s_nick(e, strtrim(crd.nick), strlentrim(crd.nick));

    _c2s_connect(e, crd.username[0] ? crd.username : NULL, NULL,
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
