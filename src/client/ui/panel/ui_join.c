#include <client/ui/panel.h>

void ui_join_from_side_channel(GtkWidget *widget, gpointer data)
{
    const char *channel;
    t_ui_panel *ui;

    ui = (t_ui_panel *)data;

    channel = gtk_button_get_label(GTK_BUTTON(widget));

    logdebug("ui::ui_join_from_side_channel:: %s\n", channel);

    // When already into channel
    if (memcmp(ui->e->fds[ui->e->sock].channelname, channel, strlen(channel)) ==
        0)
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
    GList *     children;
    const char *label;
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
    
    ui->channels_count--;

    children = gtk_container_get_children(GTK_CONTAINER(ui->channels_box));

    label = gtk_button_get_label(
        GTK_BUTTON(gtk_bin_get_child(GTK_BIN(children->data))));
    g_list_free(children);

    _c2s_join(&ui->e->fds[ui->e->sock], label, strlen(label));

    return (0);
}

int ui_join(t_ui_panel *ui, const char *channel)
{
    GtkWidget *w;

    logdebug("ui::ui_join:: %s\n", channel);

    ui_chat_empty_chat_box(ui);

    ui_set_channel_name(ui, channel);

    ui->channels_box =
        GTK_WIDGET(gtk_builder_get_object(ui->builder, "channels_box"));

    if (ui_join_channel_list_index_of(ui->channels_box, channel,
                                      strlen(channel)) != -1)
        return (0);

    ui->channels_count++;

    w = gtk_button_new();
    // gtk_widget_set_app_paintable(w, TRUE);

    gtk_button_set_label(GTK_BUTTON(w), channel);
    gtk_set_class(w, "channels-box-item");
    g_signal_connect(w, "clicked", G_CALLBACK(ui_join_from_side_channel), ui);
    gtk_list_box_insert(GTK_LIST_BOX(ui->channels_box), w, -1);

    gtk_widget_show_all(ui->channels_box);

    return (0);
}
