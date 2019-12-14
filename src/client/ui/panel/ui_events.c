/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_events.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:30:05 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:30:06 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

gboolean	on_keypress(
		GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	(void)widget;
	if (event->keyval == GDK_KEY_Escape)
		gtk_main_quit();
	else if (event->keyval == GDK_KEY_l && (event->state & GDK_CONTROL_MASK))
		ui_chat_empty_chat_box(data);
	return FALSE;
}

void		on_entry(GtkWidget *widget, gpointer data)
{
	c2s(data, gtk_entry_get_text(GTK_ENTRY(widget)));
	gtk_entry_set_text(GTK_ENTRY(widget), "");
}
