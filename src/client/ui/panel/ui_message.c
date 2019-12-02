#include <client/ui/panel.h>

void ui_push_chat_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *container;
    GtkWidget *label;
    GtkWidget *hour;
    char       h[ISOTIMESTRSIZE];

    (void)ui;

    // Container
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_set_class(container, "chat-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Hour content
    fmttime(h, "%H:%M %p");
    hour = gtk_label_new(h);
    gtk_label_set_selectable(GTK_LABEL(hour), TRUE);
    gtk_label_set_xalign(GTK_LABEL(hour), 0);
    gtk_set_class(hour, "chat-message-hour");

    // Message content
    label = gtk_label_new(msg);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "chat-message");

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
    GtkWidget *logo;
    GtkWidget *label;

    logdebug("ui::ui_push_topic_message\n");

    // Container
    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "topic-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Logo
    logo = gtk_image_new();
    gtk_image_set_from_file(GTK_IMAGE(logo), ui->topic_image);
    gtk_set_class(logo, "topic-logo");

    // Message content
    label = gtk_label_new(msg);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    // gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "topic-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
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
    GtkWidget *logo;
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "error-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Logo
    logo = gtk_image_new();
    gtk_image_set_from_file(GTK_IMAGE(logo), ui->error_image);
    gtk_set_class(logo, "error-logo");

    // Message content
    label = gtk_label_new(msg);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    // gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "error-message");

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
    GtkWidget *container;
    GtkWidget *logo;
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "info-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Logo
    logo = gtk_image_new();
    gtk_image_set_from_file(GTK_IMAGE(logo), ui->info_image);
    gtk_set_class(logo, "info-logo");

    // Message content
    label = gtk_label_new(msg);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    // gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "info-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}

void ui_push_away_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *container;
    GtkWidget *logo;
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "away-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Logo
    logo = gtk_image_new();
    gtk_image_set_from_file(GTK_IMAGE(logo), ui->info_image);
    gtk_set_class(logo, "away-logo");

    // Message content
    label = gtk_label_new(msg);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    // gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "away-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}
