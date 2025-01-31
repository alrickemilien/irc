#include <client/ui/panel.h>

GtkWidget *ui_new_topic_message(t_ui_panel *ui, const char *msg)
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

    return (container);
}

GtkWidget *ui_new_error_message(t_ui_panel *ui, const char *msg)
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

    return (container);
}

GtkWidget *ui_new_invite_message(t_ui_panel *ui, const char *msg)
{
    GtkWidget *container;
    GtkWidget *label;

    logdebug("ui::ui_push_invite_message\n");

    // Container
    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "invite-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Message content
    label = ui_util_new_chat_label(msg, "invite-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container),
                       ui_util_new_logo(ui->invite_image, "invite-logo"), FALSE,
                       FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    return (container);
}

GtkWidget *ui_new_info_message(t_ui_panel *ui, const char *msg)
{
    GtkWidget *container_1;
    GtkWidget *container_2;
    GtkWidget *label;
    GtkWidget *logo;

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
    logo = ui_util_new_logo(ui->info_image, "info-logo");
    gtk_box_pack_start(GTK_BOX(container_2), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container_2), container_1, FALSE, FALSE, 0);

    return (container_2);
}

GtkWidget *ui_new_away_message(t_ui_panel *ui, const char *msg)
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

    return (container);
}
