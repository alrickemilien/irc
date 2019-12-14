/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_chat_scroll_to_bottom.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 14:04:24 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 14:04:24 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

gboolean			ui_chat_scroll_to_bottom(gpointer w)
{
	GtkAdjustment	*adjust;

	adjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(w));
	gtk_adjustment_set_value(adjust, gtk_adjustment_get_upper(adjust));
	gtk_widget_show_all(w);
	return (FALSE);
}
