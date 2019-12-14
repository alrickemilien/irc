/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:21:35 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:27:32 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

int		ui_join_channels_index_of(t_ui_panel *ui,
		const char *needle,
		size_t needle_size)
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

void	ui_join_from_side_channel(
		GtkWidget *widget, gpointer data)
{
	const char *channel;
	t_ui_panel *ui;

	ui = (t_ui_panel *)data;
	channel = gtk_button_get_label(GTK_BUTTON(widget));
	if (strcmp(ui->e->fds[ui->e->sock].channelname, channel) == 0)
		return ;
	do_c2s_join(&ui->e->fds[ui->e->sock], channel, strlen(channel));
}

int		ui_join_channel_list_index_of(GtkWidget *list,
		const char *needle,
		size_t needle_size)
{
	GList		*children;
	GList		*iter;
	int			i;
	const char	*label;

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

void	insert_channel_left_panel(
		t_ui_panel *ui, const char *channel)
{
	GtkWidget	*w;

	w = gtk_button_new();
	gtk_button_set_label(GTK_BUTTON(w), channel);
	gtk_set_class(w, "channels-box-item");
	g_signal_connect(w, "clicked", G_CALLBACK(ui_join_from_side_channel), ui);
	gtk_list_box_insert(GTK_LIST_BOX(ui->channels_box), w, -1);
	gtk_widget_show_all(ui->channels_box);
	gtk_set_cursor_style(w, "pointer");
}

int		ui_join(t_ui_panel *ui, const char *channel)
{
	int index;

	ui_set_channel_name(ui, channel);
	index = ui_join_channels_index_of(ui, channel, strlen(channel));
	if (index != -1)
	{
		if (ui->channel_index == index)
			return (0);
		gtk_container_remove(GTK_CONTAINER(ui->chat_box_viewport),
				ui->channels[ui->channel_index].chat_box);
		ui->channel_index = index;
		gtk_container_add(GTK_CONTAINER(ui->chat_box_viewport),
				ui->channels[ui->channel_index].chat_box);
		g_object_ref(ui->channels[ui->channel_index].chat_box);
		g_timeout_add(50, ui_chat_scroll_to_bottom, ui->scrollwin);
		return (0);
	}
	if (ui->channels_count == UI_CHAT_MAX)
		return (logerror("Channel count max has been reached"));
	insert_channel_left_panel(ui, channel);
	if (ui->channel_index != -1)
		gtk_container_remove(GTK_CONTAINER(ui->chat_box_viewport),
				ui->channels[ui->channel_index].chat_box);
	ui->channel_index++;
	ui->channels[ui->channel_index].chat_box = gtk_list_box_new();
	gtk_container_add(GTK_CONTAINER(ui->chat_box_viewport),
			ui->channels[ui->channel_index].chat_box);
	g_object_ref(ui->channels[ui->channel_index].chat_box);
	strcpy(ui->channels[ui->channel_index].label, channel);
	memset(ui->channels[ui->channel_index].chat_msg_bloc_list, 0,
			sizeof(ui->channels[ui->channel_index].chat_msg_bloc_list));
	ui->channels_count++;
	return (0);
}
