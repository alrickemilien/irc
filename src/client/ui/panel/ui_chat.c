#include <client/ui/panel.h>

void ui_push_chat_message(t_ui_panel *        ui,
                          t_ui_chat_msg_bloc *bloc,
                          const char *        msg)
{
    GtkWidget *w;

    (void)ui;

    w = gtk_label_new(msg);
    gtk_set_class(w, "chat-message");
    // Specific: it seems that text alignment / justification
    // of GtkLabel can not be set through CSS (yet)
    gtk_label_set_xalign(GTK_LABEL(w), 0);
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), w, -1);
    bloc->count++;
}

void ui_push_topic_message(t_ui_panel *        ui,
                           t_ui_chat_msg_bloc *bloc,
                           const char *        msg)
{
    GtkWidget *container;
    GtkWidget *logo;
    GtkWidget *label;

    container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_set_class(container, "topic-message-container");
    gtk_box_set_homogeneous(GTK_BOX(container), FALSE);

    // Logo
    logo = gtk_image_new();
    gtk_image_set_from_file(GTK_IMAGE(logo), ui->topic_image);
    gtk_set_class(logo, "topic-logo");

    // Message content
    label = gtk_label_new(msg);
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
    // gtk_label_set_xalign(GTK_LABEL(label), 0);
    gtk_set_class(label, "error-message");

    // Fill container
    gtk_box_pack_start(GTK_BOX(container), logo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(container), label, FALSE, FALSE, 0);

    // Finally insert
    gtk_list_box_insert(GTK_LIST_BOX(bloc->box), container, -1);

    bloc->count++;
}

// void on_full(GtkWidget *widget, GtkAllocation *allocation, void *data) {
//     printf("width = %d, height = %d\n", allocation->width,
//     allocation->height);
// }

// g_signal_connect(mywidget, "size-allocate", G_CALLBACK(my_getsize), NULL);

void ui_chat_scroll_to_bottom(t_ui_panel *ui)
{
    static gdouble last_upper = 0;

    GtkAdjustment *verticalAdjust;
    gdouble        adjust = 0;
    gdouble        lower = 0;
    gdouble        upper = 0;
    gdouble        page_size = 0;
    gdouble        step_size = 0.1;

    ui->scrollwin =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "scrollwin"));
    gtk_widget_show_all(ui->scrollwin);

    verticalAdjust =
        gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin));
    upper = gtk_adjustment_get_upper(verticalAdjust);

    if (last_upper == 0)
        last_upper = upper;
    else if (last_upper == upper)
        return;

    lower = gtk_adjustment_get_lower(verticalAdjust);
    upper = gtk_adjustment_get_upper(verticalAdjust);
    page_size = gtk_adjustment_get_page_size(verticalAdjust);
    step_size = ((upper - page_size) - lower) / page_size;

    // logdebug("lower: %f\n", lower);
    // logdebug("upper: %f\n", upper);
    // logdebug("page_size: %f\n", page_size);
    // logdebug("step_size: %f\n", step_size);
    // logdebug("last_upper: %f\n", last_upper);

    adjust = upper - page_size - 1;

    // logdebug("adjust: %f\n", adjust);
    // logdebug("new adjust: %f\n",
    //          upper - page_size + (2 * upper - 2 * last_upper));

    gtk_adjustment_set_value(verticalAdjust,
                             upper - page_size + (upper - last_upper));
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(ui->scrollwin),
                                        verticalAdjust);

    last_upper = upper;
    gtk_widget_show_all(ui->scrollwin);
}

void ui_chat_empty_viewport(t_ui_panel *ui)
{
    // GList *children;
    // GList *item;

    // ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    // if (ui->chat_box == NULL)
    //     return;

    // // Delete first element
    // children = gtk_container_get_children(GTK_CONTAINER(ui->chat_box));
    // item = children;
    // while (item)
    // {
    //     gtk_widget_destroy(GTK_WIDGET(item->data));
    //     item = item->next;
    // }
    // g_list_free(children);
    // gtk_widget_show_all(ui->chat_box);
}

void ui_new_message(t_ui_panel *ui, const char *msg, int type)
{
    t_ui_chat_msg_bloc *bloc;
    size_t              i;
    GList *             children;
    // char                t[ISOTIMESTRSIZE];

    // Message size + ISO date
    // char label[510 + ISOTIMESTRSIZE];

    logdebug("ui::ui_new_chat_message:: %s\n", msg);

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    ui->msg_count++;

    i = 0;
    while (i < UI_CHAT_BOX_BLOC_MAX && ui->chat_msg_bloc_list[i].box &&
           ui->chat_msg_bloc_list[i].count == UI_CHAT_BOX_MSG_COUNT_MAX)
        i++;

    // When bloc msg list is full
    if (i < UI_CHAT_BOX_BLOC_MAX && ui->chat_msg_bloc_list[i].box == NULL)
    {
        ui->chat_msg_bloc_list[i].box = gtk_list_box_new();
        gtk_set_class(ui->chat_msg_bloc_list[i].box, "chat-message-box");
        ui->chat_msg_bloc_list[i].count = 0;
        bloc = &ui->chat_msg_bloc_list[i];

        // Insert into list of bloc message
        gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), bloc->box, -1);
    }
    else if (i == UI_CHAT_BOX_BLOC_MAX)
    {
        // Delete first element
        children = gtk_container_get_children(GTK_CONTAINER(ui->chat_box));
        gtk_widget_destroy(GTK_WIDGET(children->data));
        g_list_free(children);

        i = 0;
        while (i < (UI_CHAT_BOX_BLOC_MAX - 1))
        {
            ui->chat_msg_bloc_list[i].box = ui->chat_msg_bloc_list[i + 1].box;
            ui->chat_msg_bloc_list[i].count =
                ui->chat_msg_bloc_list[i + 1].count;

            // // Insert into list of bloc message
            // gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box),
            //                     ui->chat_msg_bloc_list[i + 1].box, -1);
            i++;
        }

        bloc = &ui->chat_msg_bloc_list[i];
        bloc->box = gtk_list_box_new();
        gtk_set_class(bloc->box, "chat-message-box");
        bloc->count = 0;

        // Insert into list of bloc message
        gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), bloc->box, -1);
    }
    else
    {
        bloc = &ui->chat_msg_bloc_list[i];
    }

    // memset(label, msg, strlen(msg));
    // memset(label, msg, strlen(msg));

    // time2iso(t);
    // printf(is_tty ? "[%s] "
    //                 "\x1b[31m"
    //                 "ERROR:"
    //                 "\x1b[0m"
    //                 " %s: %s\n"
    //               : "[%s] ERROR: %s: %s\n",
    //        t, str,
    //        errno | h_errno ? strerror(errno | h_errno) : "Unknown error");

    switch (type)
    {
        case UI_CHAT_MSG:
            ui_push_chat_message(ui, bloc, msg);
            break;
        case UI_TOPIC_MSG:
            ui_push_topic_message(ui, bloc, msg);
            break;
        case UI_ERROR_MSG:
            ui_push_error_message(ui, bloc, msg);
            break;
        default:
            break;
    }

    // if (bloc->count == CHAT_BOX_MSG_COUNT_MAX)
    // {
    //     w = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    //     gtk_set_class(ui->chat_msg_bloc_list[i].box,
    //                   "chat-message-box-separator");
    //     gtk_list_box_insert(GTK_LIST_BOX(ui->chat_box), w, -1);
    // }

    // End by scroll and show
    ui_chat_scroll_to_bottom(ui);
    gtk_widget_show_all(ui->chat_box);
}
