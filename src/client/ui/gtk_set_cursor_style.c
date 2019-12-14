/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_set_cursor_style.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:28:07 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:28:07 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/ui.h>

void	gtk_set_cursor_style(
	GtkWidget *w, const char *cursor_type)
{
	GdkCursor	*cursor;

	cursor = gdk_cursor_new_from_name(
			gdk_display_get_default(),
			cursor_type);
	gdk_window_set_cursor(gtk_widget_get_window(w), cursor);
}
