/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gtk_set_class.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:31:16 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:31:17 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/ui.h>

void	gtk_set_class(GtkWidget *w, const char *class)
{
	GtkStyleContext	*context;

	context = gtk_widget_get_style_context(w);
	gtk_style_context_add_class(context, class);
}
