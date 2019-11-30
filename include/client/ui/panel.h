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

    // Assets
    char                *status_ok_image;
    char                *status_not_ok_image;
    char                *status_away_image;
    char                *topic_image;
    char                *error_image;
    char                *info_image;
    int                 msg_count;

    t_ui_chat_msg_bloc  chat_msg_bloc_list[5]; // 5 arbitrary used

    t_env               *e;
}                       t_ui_panel;

typedef enum            e_ui_msg_type {
    UI_CHAT_MSG,
    UI_TOPIC_MSG,
    UI_ERROR_MSG,
    UI_INFO_MSG,
}                       t_ui_msg_type;

int                     ui_init_panel_window(t_env *e, t_ui_panel *ui);
void                    ui_new_message(t_ui_panel *ui, const char *str, int type);
void                    set_channel_name(t_ui_panel *ui, const char *msg);
void                    ui_set_nick(t_ui_panel *ui, const char *msg);
void                    set_user_name(t_ui_panel *ui, const char *msg);
int                     ui_set_status(t_ui_panel *ui, int status);
int                     ui_join(t_ui_panel *ui, const char *channel);
int                     ui_away(t_ui_panel *ui, const char *channel);
int                     ui_unaway(t_ui_panel *ui);
int                     ui_topic(t_ui_panel *ui, const char *msg);
void                    ui_chat_empty_viewport(t_ui_panel *ui);
int                     ui_whois(t_ui_panel *ui, const char *msg);

#endif
