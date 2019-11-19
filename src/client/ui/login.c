#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/ui.h>

GtkWidget *host_entry;
GtkWidget *port_entry;
GtkWidget *username_entry;
GtkWidget *pass_entry;
GtkWidget *button_go;

void login_window_init(t_env *e)
{
    builder = gtk_builder_new();

    if (gtk_builder_load(builder, e->argv_0, "/ui/login.glade") < 0)
        return;

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    host_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_host"));
    port_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_port"));
    username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
    pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_pass"));

    button_go = GTK_WIDGET(gtk_builder_get_object(builder, "button_go"));
    g_signal_connect(button_go, "clicked", G_CALLBACK(login_connect), e);
}

void login_connect(GtkWidget *widget, gpointer data)
{
    t_env *     e;
    // const char *host_data;
    // const char *port_data;
    // const char *username_data;
    // const char *pass_data;
    // const char  concat[512];

    (void)widget;
    e = (t_env *)data;

    // host_data = gtk_entry_get_text(GTK_ENTRY(host_entry));
    // port_data = gtk_entry_get_text(GTK_ENTRY(port_entry));
    // username_data = gtk_entry_get_text(GTK_ENTRY(username_entry));
    // pass_data = gtk_entry_get_text(GTK_ENTRY(pass_entry));

    if (e->sock == -1)
    {
        logerrno("main:");
        return;
    }

    // memset(concat, 0, sizeof(concat));
    // sprintf(concat, "/pass %s\x0D\x0A", pass_data);
    // c2s_pass(e, e->sock, concat);

    // memset(concat, 0, sizeof(concat));
    // sprintf(concat, "/nick %s\x0D\x0A", username_data);
    // c2s_nick(e, e->sock, concat);

    // memset(concat, 0, sizeof(concat));
    // sprintf(concat, "/connect %s %s\x0D\x0A", host_data, port_data);
    // c2s_connect(e, e->sock, concat);

    if (e->sock != -1)
        XSAFE(-1, close(e->sock), "login_connect::close");

    /* received flag */
    // recv(socket_fd, &flag, 2, 0);
    // flag = ntohs(flag);
    // printf("received flag=%d\n", flag);
    // if (0 == flag)
    // { /* 0 is login success, -1 is failed */

    // gtk_label_set_text(GTK_LABEL(label), "loading...");

    gtk_widget_hide(window);

    // mainWindow();
    // }
    // else
    // {
    //     gtk_label_set_text(GTK_LABEL(label),
    // 			"loading failed,username or password error!");
    //     gtk_entry_set_text(GTK_ENTRY(passwordEntry), "");
    // }
}
