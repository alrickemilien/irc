#ifndef CLIENT_UI_LOGIN_H
#define CLIENT_UI_LOGIN_H

#include "client/ui/ui.h"

GtkWidget *login_window(t_env *e);
void login_connect(GtkWidget *widget, gpointer data);

#endif
