/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_chat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:53:55 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:53:56 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

void				ui_chat_empty_chat_box(t_ui_panel *ui)
{
	t_ui_channel	*ch;
	GList			*children;
	GList			*item;

	if (ui->channel_index == -1)
		return ;
	ch = &ui->channels[ui->channel_index];
	ch->msg_count = 0;
	children = gtk_container_get_children(GTK_CONTAINER(ch->chat_box));
	item = children;
	while (item)
	{
		gtk_widget_destroy(GTK_WIDGET(item->data));
		item = g_list_next(item);
	}
	g_list_free(children);
	memset(&ch->chat_msg_bloc_list, 0, sizeof(ch->chat_msg_bloc_list));
}

static void			ui_message_dispatch(t_ui_panel *ui,
		t_ui_chat_msg_bloc *bloc,
		const char *msg,
		int type)
{
	GtkWidget	*w;

	w = NULL;
	if (type == UI_CHAT_MSG)
		w = ui_new_chat_message(ui, msg);
	else if (type == UI_TOPIC_MSG)
		w = ui_new_topic_message(ui, msg);
	else if (type == UI_ERROR_MSG)
		w = ui_new_error_message(ui, msg);
	else if (type == UI_INFO_MSG)
		w = ui_new_info_message(ui, msg);
	else if (type == UI_AWAY_MSG)
		w = ui_new_away_message(ui, msg);
	else if (type == UI_INVITE_MSG)
		w = ui_new_invite_message(ui, msg);
	if (w == NULL)
		return ;
	gtk_list_box_insert(GTK_LIST_BOX(bloc->box), w, -1);
	bloc->count++;
}

t_ui_chat_msg_bloc	*ui_new_message_rotate(t_ui_channel *ch)
{
	GList				*children;
	size_t				i;
	t_ui_chat_msg_bloc	*bloc;

	children = gtk_container_get_children(GTK_CONTAINER(ch->chat_box));
	gtk_widget_destroy(GTK_WIDGET(children->data));
	g_list_free(children);
	i = 0;
	while (i < (UI_CHAT_BOX_BLOC_MAX - 1))
	{
		ch->chat_msg_bloc_list[i].box = ch->chat_msg_bloc_list[i + 1].box;
		ch->chat_msg_bloc_list[i].count =
			ch->chat_msg_bloc_list[i + 1].count;
		i++;
	}
	bloc = &ch->chat_msg_bloc_list[i];
	bloc->box = gtk_list_box_new();
	gtk_set_class(bloc->box, "chat-message-box");
	bloc->count = 0;
	gtk_list_box_insert(GTK_LIST_BOX(ch->chat_box), bloc->box, -1);
	return (bloc);
}

t_ui_chat_msg_bloc	*ui_new_message_new_block(t_ui_channel *ch, size_t i)
{
	t_ui_chat_msg_bloc	*bloc;

	ch->chat_msg_bloc_list[i].box = gtk_list_box_new();
	gtk_set_class(ch->chat_msg_bloc_list[i].box, "chat-message-box");
	ch->chat_msg_bloc_list[i].count = 0;
	bloc = &ch->chat_msg_bloc_list[i];
	gtk_list_box_insert(GTK_LIST_BOX(ch->chat_box), bloc->box, -1);
	return (bloc);
}

void				ui_new_message(
		t_ui_panel *ui, const char *msg, int type)
{
	t_ui_channel		*ch;
	t_ui_chat_msg_bloc	*bloc;
	size_t				i;

	if (ui->channel_index == -1)
		return ;
	ch = &ui->channels[ui->channel_index];
	ch->msg_count++;
	i = 0;
	bloc = NULL;
	while (i < UI_CHAT_BOX_BLOC_MAX && ch->chat_msg_bloc_list[i].box &&
			ch->chat_msg_bloc_list[i].count == UI_CHAT_BOX_MSG_COUNT_MAX)
		i++;
	if (i < UI_CHAT_BOX_BLOC_MAX && ch->chat_msg_bloc_list[i].box == NULL)
		bloc = ui_new_message_new_block(ch, i);
	else if (i == UI_CHAT_BOX_BLOC_MAX)
		bloc = ui_new_message_rotate(ch);
	else
		bloc = &ch->chat_msg_bloc_list[i];
	ui_message_dispatch(ui, bloc, msg, type);
	g_timeout_add(50, ui_chat_scroll_to_bottom, ui->scrollwin);
	gtk_widget_show_all(ch->chat_box);
	gtk_widget_show_all(ui->scrollwin);
}
