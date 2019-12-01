#include <client/ui/panel.h>

gboolean ui_chat_scroll_to_bottom(gpointer w)
{
    GtkAdjustment *adjust;

    adjust = gtk_scrolled_window_get_vadjustment
        (GTK_SCROLLED_WINDOW (w));
    gtk_adjustment_set_value (adjust, gtk_adjustment_get_upper (adjust));

    gtk_widget_show_all(w);

    return (FALSE);

}

void ui_chat_empty_chat_box(t_ui_panel *ui)
{
    GList *children;
    GList *item;

    logdebug("ui::ui_chat_empty_chat_box\n");

    ui->chat_box = GTK_WIDGET(gtk_builder_get_object(ui->builder, "chat_box"));

    if (ui->chat_box == NULL)
        return;

    // Delete first element
    children = gtk_container_get_children(GTK_CONTAINER(ui->chat_box));
    item = children;
    while (item)
    {
        gtk_widget_destroy(GTK_WIDGET(item->data));
        item = g_list_next(item);
    }

    memset(ui->chat_msg_bloc_list, 0, sizeof(ui->chat_msg_bloc_list));
    ui->msg_count = 0;

    g_list_free(children);
    gtk_widget_show_all(ui->chat_box);

    logdebug("ui::ui_chat_empty_chat_box::leave\n");
}

void ui_new_message(t_ui_panel *ui, const char *msg, int type)
{
    t_ui_chat_msg_bloc *bloc;
    size_t              i;
    GList *             children;

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
        case UI_INFO_MSG:
            ui_push_info_message(ui, bloc, msg);
            break;
        default:
            break;
    }

    g_timeout_add(50, ui_chat_scroll_to_bottom, ui->scrollwin);

    gtk_widget_show_all(ui->chat_box);
    gtk_widget_show_all(ui->scrollwin);

    // End by scroll and show
    // ui_chat_scroll_to_bottom(ui);
}