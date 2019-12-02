#include <client/ui/panel.h>

int ui_leave_channel_list(GtkWidget * list,
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

int ui_leave(t_ui_panel *ui, const char *channel)
{
    logdebug("ui::ui_leave:: %s\n", channel);

    ui->channels_box =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    logdebug("ui->channels_count: %ld\n", ui->channels_count);
    if (ui->channels_count == 1)
    {
        ui_new_message(ui, "You can't leave primary channel", UI_ERROR_MSG);
        return (-1);
    }

    if (ui_leave_channel_list(ui->channels_box, channel, strlen(channel)) < 0)
        return (-1);

    _c2s_join(&ui->e->fds[ui->e->sock], ui->channels[ui->channels_count].label,
              strlen(ui->channels[ui->channels_count].label));

    return (0);
}

void ui_join_from_side_channel(GtkWidget *widget, gpointer data)
{
    const char *channel;
    t_ui_panel *ui;

    ui = (t_ui_panel *)data;

    channel = gtk_button_get_label(GTK_BUTTON(widget));

    logdebug("ui::ui_join_from_side_channel:: %s\n", channel);

    // When already into channel
    if (strcmp(ui->e->fds[ui->e->sock].channelname, channel) == 0)
        return;

    _c2s_join(&ui->e->fds[ui->e->sock], channel, strlen(channel));
}

int ui_join_channel_list_index_of(GtkWidget * list,
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
            g_list_free(children);
            return (i);
        }

        iter = g_list_next(iter);

        i++;
    }

    g_list_free(children);

    return (-1);
}

int ui_join_channels_index_of(t_ui_panel *ui,
                              const char *needle,
                              size_t      needle_size)
{
    int i;

    i = 0;
    while (i < ui->channels_count)
    {
        if (memcmp(ui->channels[i].label, needle, needle_size) == 0 &&
            (ui->channels[i].label[needle_size] == ' ' ||
             ui->channels[i].label[needle_size] == 0))
            return (i);
        i++;
    }

    return (-1);
}

int ui_join(t_ui_panel *ui, const char *channel)
{
    GtkWidget *w;
    int        index;

    logdebug("ui::ui_join:: %s", channel);

    if (ui->channels_count == UI_CHAT_MAX)
        return (logerror("Channel count max has been reached"));

    ui_set_channel_name(ui, channel);

    index = ui_join_channels_index_of(ui, channel, strlen(channel));

    // When it already exists, just set the index to the selected one
    if (index != -1)
    {
        ui->channel_index = index;

        // Need to hide other boxes and show this one

        return (0);
    }

    logdebug("A");

    // push button on left side
    w = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(w), channel);
    gtk_set_class(w, "channels-box-item");
    g_signal_connect(w, "clicked", G_CALLBACK(ui_join_from_side_channel), ui);
    gtk_list_box_insert(GTK_LIST_BOX(ui->channels_box), w, -1);
    gtk_widget_show_all(ui->channels_box);

    logdebug("B");

    // create the chatbox
    if (ui->channel_index != -1)
        gtk_container_remove(GTK_CONTAINER(ui->chat_box_viewport),
                             ui->channels[ui->channel_index].chat_box);

    ui->channels[++ui->channel_index].chat_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(ui->chat_box_viewport),
                      ui->channels[ui->channel_index].chat_box);

    ui->channels_count++;

    return (0);
}
