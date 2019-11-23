#ifndef CLIENT_UI_PANEL_H
#define CLIENT_UI_PANEL_H

#include <client/ui/ui.h>

GtkWidget   *panel_window(t_env *e);
void        new_chat_message(const char *str);
void        set_channel_name(const char *msg);
void        set_nick_name(const char *msg);

#endif
