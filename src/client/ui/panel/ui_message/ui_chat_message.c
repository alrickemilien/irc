#include <client/ui/panel.h>

/*
** Extract prefixed nickname from msg
*/

static GtkWidget *ui_extract_nick(const char *msg, int dir)
{
    size_t      i;
    size_t      j;
    const char *nick_end = strchr(msg, ':');
    char        nick_str[NICKNAMESTRSIZE + 2];

    if (!nick_end)
        return NULL;

    i = 0;
    if (dir == GTK_TEXT_DIR_RTL)
        nick_str[i++] = ':';

    j = 0;
    while (i < (NICKNAMESTRSIZE + 1) && nick_end != &msg[j])
        nick_str[i++] = msg[j++];

    if (dir == GTK_TEXT_DIR_LTR)
        nick_str[i++] = ':';

    nick_str[i] = 0;
    return (ui_util_new_chat_label(nick_str, "chat-nick"));
}

/*
** Build nick/msg container pair according to rtl or ltr
*/

static GtkWidget *ui_build_nick_msg_pair(const char *msg, int is_rtl)
{
    GtkWidget *container;
    GtkWidget *label;
    GtkWidget *nick;

    // Sub Container
    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Message content
    label = ui_util_new_chat_label(strchr(msg, ':') + 1, "chat-message");

    if (is_rtl)
    {
        // Nick content
        nick = ui_extract_nick(msg, GTK_TEXT_DIR_RTL);

        gtk_label_set_xalign(GTK_LABEL(label), 1);
        gtk_label_set_xalign(GTK_LABEL(nick), 1);

        gtk_box_pack_end(GTK_BOX(container), GTK_WIDGET(nick), FALSE, FALSE, 0);
        gtk_box_pack_end(GTK_BOX(container), GTK_WIDGET(label), FALSE, FALSE,
                         0);
    }
    else
    {
        // Nick content
        nick = ui_extract_nick(msg, GTK_TEXT_DIR_LTR);

        gtk_label_set_xalign(GTK_LABEL(label), 0);
        gtk_label_set_xalign(GTK_LABEL(nick), 0);

        gtk_box_pack_start(GTK_BOX(container), GTK_WIDGET(nick), FALSE, FALSE,
                           0);
        gtk_box_pack_start(GTK_BOX(container), GTK_WIDGET(label), FALSE, FALSE,
                           0);
    }

    return (container);
}

GtkWidget *ui_new_chat_message(t_ui_panel *ui, const char *msg)
{
    int        is_rtl;
    GtkWidget *container;
    GtkWidget *nick_msg_pair;
    GtkWidget *hour;

    (void)ui;

    is_rtl =
        (gtk_pango_find_base_dir(strchr(msg, ':'), strlen(strchr(msg, ':'))) ==
         PANGO_DIRECTION_RTL);

    nick_msg_pair = ui_build_nick_msg_pair(msg, is_rtl);

    // Hour
    hour = ui_util_new_hour_label("%H:%M %p", "chat-message-hour");
    if (is_rtl)
        gtk_label_set_xalign(GTK_LABEL(hour), 1);

    // Container
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_set_class(container, "chat-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), hour, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), nick_msg_pair, FALSE, FALSE, 0);

    return (container);
}