#include <client/ui/panel.h>

GtkWidget *ui_new_chat_message(t_ui_panel *ui, const char *msg)
{
    GtkWidget *container;
    GtkWidget *label;
    GtkWidget *nick;
    GtkWidget *hour;
    GtkWidget *sub_container;

    (void)ui;

    // Hour
    hour = ui_util_new_hour_label("%H:%M %p", "chat-message-hour");

    // Message content
    label = ui_util_new_chat_label(msg, "chat-message");
    // Nick content
    nick = ui_util_new_chat_label(msg, "chat-nick");

    // Sub container
    sub_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(sub_container), FALSE);

    if (ui_handle_i18n_direction((label), hour, strchr(msg, ':')) == GTK_TEXT_DIR_RTL)
    {
        gtk_box_pack_start(GTK_BOX(sub_container), nick, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(sub_container), label, FALSE, FALSE, 0);
    }
    else
    {
        gtk_box_pack_start(GTK_BOX(sub_container), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(sub_container), nick, FALSE, FALSE, 0);
    }

    // Container
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_set_class(container, "chat-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), hour, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), sub_container, FALSE, FALSE, 0);

    return (container);
}