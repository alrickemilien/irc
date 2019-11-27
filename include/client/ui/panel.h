#ifndef CLIENT_UI_PANEL_H
#define CLIENT_UI_PANEL_H

#include <client/ui/ui.h>

typedef struct  s_ui_panel {
    GtkWidget   *window;
    GdkRGBA     *window_color;
    GtkBuilder  *builder;

    GtkWidget   *channel_label;
    GtkWidget   *nick_label;
    GtkWidget   *user_label;
    GtkWidget   *chat_entry;

    GtkWidget   *status_image;

    GtkWidget   *scrollwin;
    GtkWidget   *chat_box;

    char        *status_ok_image;
    char        *status_not_ok_image;
    int         msg_count;

    t_env       *e;
}               t_ui_panel;

int             ui_init_panel_window(t_env *e, t_ui_panel *ui);
void            new_chat_message(t_ui_panel *ui, const char *str);
void            set_channel_name(t_ui_panel *ui, const char *msg);
void            set_nick_name(t_ui_panel *ui, const char *msg);
void            set_user_name(t_ui_panel *ui, const char *msg);
int             set_status(t_ui_panel *ui, int status);

#endif
