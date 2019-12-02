#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ui/login.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static void ui_login_connect_fetch_entrys(t_ui_login *ui)
{
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
}

static int ui_login_connect_fetch_entrys_content(t_env *           e,
                                                 t_ui_login *      ui,
                                                 t_ui_credentials *crd)
{
    crd->host = gtk_entry_get_text(GTK_ENTRY(ui->host_entry));
    crd->port = gtk_entry_get_text(GTK_ENTRY(ui->port_entry));
    crd->username = gtk_entry_get_text(GTK_ENTRY(ui->username_entry));
    crd->pass = gtk_entry_get_text(GTK_ENTRY(ui->pass_entry));
    crd->nick = gtk_entry_get_text(GTK_ENTRY(ui->nick_entry));

    // printf("pass_data: %s\n", crd.pass);
    // printf("strtrim(pass_data): %s\n", strtrim(crd.pass));
    // printf("len(pass_data): %ld\n", strlentrim(crd.pass));
    // printf("nick_data: %s\n", crd.nick);
    // printf("strtrim(nick_data): %s\n", strtrim(crd.nick));
    // printf("len(nick_data): %ld\n", strlentrim(crd.nick));

    if (!crd->nick || crd->nick[0] == 0)
    {
        logerror("Nickname must be provided\n");
        return (-1);
    }

    // Default port
    if (ato32(crd->port[0] ? crd->port : "5555",
              (uint32_t *)&e->options.port) != 0 ||
        e->options.port < 3 || e->options.port > 99999)
    {
        logerror("port must be a vakue between 1000 an 99999'%s'.\n",
                 crd->port ? crd->port : "5555");
        return (-1);
    }

    // Default ip adress
    memcpy(e->options.host, crd->host && crd->host[0] ? crd->host : "127.0.0.1",
           crd->host && crd->host[0] ? strlen(crd->host) : strlen("127.0.0.1"));

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

    ui_login_connect_fetch_entrys(ui);

    if (ui_login_connect_fetch_entrys_content(e, ui, &crd) < 0)
        return;

    _c2s_pass(e, strtrim(crd.pass), strlentrim(crd.pass));

    _c2s_nick(e, strtrim(crd.nick), strlentrim(crd.nick));

    _c2s_connect(e, crd.username[0] ? crd.username : NULL, NULL,
                 e->options.host);

    if (e->sock != -1)
        // Load loop select
        g_idle_add((GSourceFunc)gtk_do_select, e);
    else
    {
        gtk_entry_set_text(GTK_ENTRY(ui->pass_entry), "");

        gtk_button_set_label(GTK_BUTTON(ui->button_go), "Log in");

        // gtk_label_set_text(GTK_LABEL(label), "[!] Failed to connect to
        // server");
    }
}
