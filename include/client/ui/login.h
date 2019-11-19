#ifndef CLIENT_UI_LOGIN_H
#define CLIENT_UI_LOGIN_H

#include "client/ui/ui.h"

void login_window_init(t_env *e);
void get_login_info(GtkWidget *widget, gpointer data);
void login_connect(t_env *e, const gchar *uname, const gchar *pword);

#endif
