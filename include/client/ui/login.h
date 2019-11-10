#ifndef CLIENT_UI_LOGIN_H
#define CLIENT_UI_LOGIN_H

void login_window_init();
void get_login_info(GtkWidget *widget, gpointer data);
void login_connect(const gchar *uname, const gchar *pword);

#endif