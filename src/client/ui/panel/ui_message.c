#include <client/ui/panel.h>

void ui_push_chat_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *container;
    GtkWidget *label;
    GtkWidget *hour;

    (void)ui;

    // Container
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_set_class(container, "chat-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Hour
    hour = ui_util_new_hour_label("%H:%M %p", "chat-message-hour");

    // Message content
    label = ui_util_new_chat_label(msg, "chat-message");
    gtk_label_set_xalign(GTK_LABEL(label), 0);

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), hour, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);
    bloc->count++;
}

void ui_push_topic_message(t_ui_panel *        ui,
                           t_ui_chat_msg_bloc *bloc,
                           const char *        msg)
{
    GtkWidget *container;
    GtkWidget *label;

    logdebug("ui::ui_push_topic_message\n");

    // Container
    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "topic-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Message content
    label = ui_util_new_chat_label(msg, "topic-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container),
                       ui_util_new_logo(ui->topic_image, "topic-logo"), FALSE,
                       FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}

void ui_push_error_message(t_ui_panel *        ui,
                           t_ui_chat_msg_bloc *bloc,
                           const char *        msg)
{
    GtkWidget *container;
    GtkWidget *logo = ui_util_new_logo(ui->error_image, "error-logo");
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "error-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Message content
    label = ui_util_new_chat_label(msg, "error-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}

void ui_push_info_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *container_1;
    GtkWidget *container_2;
    GtkWidget *label;

    // Container 1
    container_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    // gtk_set_class(container_1, "info-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container_1), FALSE);

    // Container 2
    container_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container_2, "info-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container_2), FALSE);

    // Message content
    label = ui_util_new_chat_label(msg, "info-message");

    // Fill container 1
    gtk_box_pack_start(GTK_BOX(container_1),
                       ui_util_new_hour_label("%H:%M %p", "chat-message-hour"),
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container_1), label, FALSE, FALSE, 0);

    // Fill container 2
    gtk_box_pack_start(GTK_BOX(container_2),
                       ui_util_new_logo(ui->info_image, "info-logo"), FALSE,
                       FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container_2), container_1, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container_2, -1);

    bloc->count++;
}

void ui_push_away_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *container;
    GtkWidget *logo = ui_util_new_logo(ui->rpl_away_image, "away-logo");
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "away-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Message content
    label = ui_util_new_chat_label(msg, "away-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}
