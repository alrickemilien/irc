#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/ui.h>

void login_window_init(t_env *e)
{
    builder = gtk_builder_new();

    if (gtk_builder_load(builder, e->argv_0, "/ui/login.xml") < 0)
        return;

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "unentry"));
    passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "pwentry"));

    label = GTK_WIDGET(gtk_builder_get_object(builder, "inform"));

    button = GTK_WIDGET(gtk_builder_get_object(builder, "loginbutton"));
    g_signal_connect(button, "clicked", G_CALLBACK(get_login_info), NULL);
}

void login_connect(t_env *e, const char *uname, const char *pword)
{
    if (e->sock == -1)
    {
        logerrno("main:");
        return;
    }

    c2s_pass(e, e->sock, pword);
    c2s_nick(e, e->sock, uname);
    c2s_connect(e, e->sock, pword);

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

/*
 * Name: get_login_info
 * Description: get username and password from ui from,
 * 		call login_connect() function.
 */
void get_login_info(GtkWidget *widget, gpointer data)
{
    const gchar *uname, *pword;

    (void)widget;

    /* get username text  */
    uname = gtk_entry_get_text(GTK_ENTRY(usernameEntry));
    pword = gtk_entry_get_text(GTK_ENTRY(passwordEntry));
    printf("username=%s\npassword=%s\n", uname, pword);
    login_connect((t_env *)data, uname, pword);
}
