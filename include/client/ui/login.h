#ifndef CLIENT_UI_LOGIN_H
#define CLIENT_UI_LOGIN_H

#include <client/ui/ui.h>

typedef struct  s_ui_login {
    GtkWidget   *window;
    GdkRGBA     *window_color;
    GtkBuilder  *builder;

    GtkWidget   *host_entry;
    GtkWidget   *port_entry;
    GtkWidget   *username_entry;
    GtkWidget   *pass_entry;
    GtkWidget   *nick_entry;

    GtkWidget   *button_go;

    t_env       *e;
}               t_ui_login;

typedef struct  s_ui_credentials {
    const char  *host;
    const char  *port;
    const char  *username;
    const char  *pass;
    const char  *nick;
}               t_ui_credentials;

int             ui_init_login(t_env *e, t_ui_login *ui);
int             ui_clear_login_window(t_ui_login *ui);
void            ui_login_connect(GtkWidget *widget, gpointer data);
gboolean        close_login(GtkWidget *widget,
		GdkEventKey *event,
		gpointer data);
#endif
