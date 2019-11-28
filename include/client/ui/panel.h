#ifndef CLIENT_UI_PANEL_H
#define CLIENT_UI_PANEL_H

#include <client/ui/ui.h>

#define UI_CHAT_BOX_BLOC_MAX 5
#define UI_CHAT_BOX_MSG_COUNT_MAX 20

typedef struct          s_ui_chat_msg_bloc {
    GtkWidget           *box;
    int                 count;
}                       t_ui_chat_msg_bloc;

typedef struct          s_ui_panel {
    GtkWidget           *window;
    GdkRGBA             *window_color;
    GtkBuilder          *builder;

    GtkWidget           *channel_label;
    GtkWidget           *nick_label;
    GtkWidget           *user_label;
    GtkWidget           *chat_entry;

    GtkWidget           *status_image;

    GtkWidget           *scrollwin;
    GtkWidget           *chat_box;

    GtkWidget           *channels_box;
    int                 channels_count;

    char                *status_ok_image;
    char                *status_not_ok_image;
    char                *status_away_image;
    int                 msg_count;

    t_ui_chat_msg_bloc  chat_msg_bloc_list[5]; // 5 arbitrary used

    t_env               *e;
}                       t_ui_panel;

int                     ui_init_panel_window(t_env *e, t_ui_panel *ui);
void                    ui_new_chat_message(t_ui_panel *ui, const char *str);
void                    set_channel_name(t_ui_panel *ui, const char *msg);
void                    set_nick_name(t_ui_panel *ui, const char *msg);
void                    set_user_name(t_ui_panel *ui, const char *msg);
int                     set_status(t_ui_panel *ui, int status);
int                     ui_join(t_ui_panel *ui, const char *channel);
int                     ui_away(t_ui_panel *ui, const char *channel);

#endif
