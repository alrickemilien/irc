#ifndef CLIENT_UI_PANEL_H
#define CLIENT_UI_PANEL_H

#include <client/ui/ui.h>

#define UI_CHAT_MAX 10
#define UI_CHAT_BOX_BLOC_MAX 5
#define UI_CHAT_BOX_MSG_COUNT_MAX 20

typedef struct          s_ui_chat_msg_bloc {
    GtkWidget           *box;
    int                 count;
}                       t_ui_chat_msg_bloc;

typedef struct          s_ui_channel {
    char                label[CHANNELSTRSIZE];
    t_ui_chat_msg_bloc  chat_msg_bloc_list[UI_CHAT_BOX_BLOC_MAX];
    int                 msg_count;
    GtkWidget           *chat_box;
}                       t_ui_channel;

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
    GtkWidget           *chat_box_viewport;

    // Channels
    t_ui_channel        channels[UI_CHAT_MAX];
    GtkWidget           *channels_box;
    int                 channels_count;
    int                 channel_index;

    // Assets
    char                *status_ok_image;
    char                *status_not_ok_image;
    char                *status_away_image;
    char                *topic_image;
    char                *error_image;
    char                *info_image;
    char                *rpl_away_image;
    char                *invite_image;

    int                 msg_count;

    t_env               *e;
}                       t_ui_panel;

typedef enum            e_ui_msg_type {
    UI_CHAT_MSG,
    UI_TOPIC_MSG,
    UI_ERROR_MSG,
    UI_INFO_MSG,
    UI_AWAY_MSG,
    UI_INVITE_MSG,
}                       t_ui_msg_type;

int                     ui_init_panel(t_env *e, t_ui_panel *ui);
gboolean                ui_chat_scroll_to_bottom(gpointer w);
void                    ui_new_message(t_ui_panel *ui, const char *str, int type);
void                    ui_set_channel_name(t_ui_panel *ui, const char *msg);
void                    ui_set_nick(t_ui_panel *ui, const char *msg);
void                    ui_set_username(t_ui_panel *ui, const char *msg);
int                     ui_set_status(t_ui_panel *ui, int status);
int                     ui_join(t_ui_panel *ui, const char *channel);
int                     ui_nowaway(t_ui_panel *ui, const char *msg);
int                     ui_away(t_ui_panel *ui, const char *msg);
int                     ui_unaway(t_ui_panel *ui);
int                     ui_topic(t_ui_panel *ui, const char *msg);
void                    ui_chat_empty_chat_box(t_ui_panel *ui);
int                     ui_leave(t_ui_panel *ui, const char *channel);
int                     ui_whois(t_ui_panel *ui, const char *msg);
int                     ui_names(t_ui_panel *ui, const char *msg);

/*
* Chat messages
*/

GtkWidget                *ui_new_chat_message(t_ui_panel *ui, const char *msg);
GtkWidget                *ui_new_topic_message(t_ui_panel *ui, const char *msg);
GtkWidget                *ui_new_error_message(t_ui_panel *ui, const char *msg);
GtkWidget                *ui_new_info_message(t_ui_panel *ui, const char *msg);
GtkWidget                *ui_new_away_message(t_ui_panel *ui, const char *msg);
GtkWidget                *ui_new_invite_message(t_ui_panel *ui, const char *msg);
int                      ui_handle_i18n_direction(GtkWidget *label, GtkWidget *hour, const char *msg);

/*
* utils
*/

int                     ui_join_channels_index_of(t_ui_panel *ui,
                              const char *needle,
                              size_t      needle_size);

/*
* Events
*/

gboolean                on_keypress(GtkWidget *  widget,
                            GdkEventKey *event,
                            gpointer     data);
void                    on_entry(GtkWidget *widget, gpointer data);

#endif
