/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:19:07 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:19:08 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/ui.h>

GtkWidget	*ui_util_new_hour_label(
	const char *fmt, const char *class)
{
	GtkWidget	*hour;
	char		h[ISOTIMESTRSIZE];

	fmttime(h, fmt);
	hour = gtk_label_new(h);
	gtk_label_set_selectable(GTK_LABEL(hour), TRUE);
	gtk_label_set_xalign(GTK_LABEL(hour), 0);
	if (class)
		gtk_set_class(hour, class);
	return (hour);
}

GtkWidget	*ui_util_new_logo(
	const char *path, const char *class)
{
	GtkWidget	*logo;

	logo = gtk_image_new();
	gtk_image_set_from_file(GTK_IMAGE(logo), path);
	if (class)
		gtk_set_class(logo, class);
	return (logo);
}

GtkWidget	*ui_util_new_chat_label(
	const char *msg, const char *class)
{
	GtkWidget	*label;

	label = gtk_label_new(msg);
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);
	if (class)
		gtk_set_class(label, class);
	return (label);
}
