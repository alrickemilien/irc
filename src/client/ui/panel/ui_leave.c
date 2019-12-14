/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_leave.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:28:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:28:41 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

int		ui_update_channel_panel_list(GtkWidget *list,
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

int		ui_leave_from_channel_list_box(t_ui_panel *ui, int index)
{
	int	i;

	g_object_unref(ui->channels[index].chat_box);
	gtk_container_remove(GTK_CONTAINER(ui->chat_box_viewport),
			ui->channels[index].chat_box);
	if (index == ui->channel_index)
	{
		gtk_container_add(GTK_CONTAINER(ui->chat_box_viewport),
				ui->channels[0].chat_box);
		g_object_ref(ui->channels[0].chat_box);
	}
	ui->channel_index = 0;
	ui->channels_count--;
	i = index;
	while (i < ui->channels_count)
	{
		ui->channels[i] = ui->channels[i + 1];
		i++;
	}
	memset(&ui->channels[ui->channels_count + 1], 0, sizeof(t_ui_channel));
	return (0);
}

int		ui_leave(t_ui_panel *ui, const char *channel)
{
	int index;

	if (ui->channels_count == 1)
	{
		ui_new_message(ui, "You can't leave primary channel", UI_ERROR_MSG);
		return (-1);
	}
	if (ui_update_channel_panel_list(ui->channels_box, channel,
				strlen(channel)) < 0)
		return (-1);
	index = ui_join_channels_index_of(ui, channel, strlen(channel));
	ui_leave_from_channel_list_box(ui, index);
	g_timeout_add(50, ui_chat_scroll_to_bottom, ui->scrollwin);
	return (0);
}
