/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:58:51 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:58:52 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/login.h>

gboolean	close_login(GtkWidget *widget,
		GdkEventKey *event,
		gpointer data)
{
	(void)widget;
	if (event->keyval == GDK_KEY_Escape)
	{
		ui_clear_login_window(data);
		gtk_main_quit();
	}
	return (FALSE);
}
