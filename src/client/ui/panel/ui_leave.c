#include <client/ui/panel.h>

 int ui_update_channel_panel_list(GtkWidget * list,
                                        const char *needle,
                                        size_t      needle_size)
{
    GList *     children;
    GList *     iter;
    int         i;
    const char *label;

    i = 0;
    children = gtk_container_get_children(GTK_CONTAINER(list));
    iter = children;
    while (iter != NULL)
    {
        label = gtk_button_get_label(
            GTK_BUTTON(gtk_bin_get_child(GTK_BIN(iter->data))));

        if (memcmp(label, needle, needle_size) == 0 &&
            (label[needle_size] == ' ' || label[needle_size] == 0))
        {
            gtk_widget_destroy(GTK_WIDGET(iter->data));
            g_list_free(children);
            return (i);
        }
        iter = g_list_next(iter);
        i++;
    }
    g_list_free(children);
    return (-1);
}

int ui_leave_from_channel_list_box(t_ui_panel *ui, int index)
{
    int i;

    // g_object_unref(ui->channels[index].chat_box);
    gtk_container_remove(GTK_CONTAINER(ui->chat_box_viewport),
                         ui->channels[index].chat_box);
    if (index == ui->channel_index)
    {
        gtk_container_add(GTK_CONTAINER(ui->chat_box_viewport),
                          ui->channels[index - 1].chat_box);
        g_object_ref(ui->channels[index - 1].chat_box);
    }

    i = index;
    while (i < (ui->channels_count - 1))
    {
        ui->channels[i] = ui->channels[i + 1];
        i++;
    }

    if (ui->channel_index >= index && ui->channel_index != 0)
        ui->channel_index--;

    ui->channels_count--;

    return (0);
}

int ui_leave(t_ui_panel *ui, const char *channel)
{
    (void)ui;
    (void)channel;
    // int index;

    // logdebug("ui::ui_leave:: %s\n", channel);

    // ui->channels_box =
    //     GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    // if (ui->channels_count == 1)
    // {
    //     ui_new_message(ui, "You can't leave primary channel", UI_ERROR_MSG);
    //     return (-1);
    // }

    // // Remove channel from left side panel list
    // if (ui_update_channel_panel_list(ui->channels_box, channel,
    //                                  strlen(channel)) < 0)
    //     return (-1);

    // index = ui_join_channels_index_of(ui, channel, strlen(channel));

    // logdebug("index: %d", index);
    // logdebug("ui->channel_index : %d", ui->channel_index);

    // ui_leave_from_channel_list_box(ui, index);

    // gtk_widget_show_all(ui->channels[ui->channel_index].chat_box);

    return (0);
}