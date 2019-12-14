/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_who.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:21:29 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:21:30 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/ui/panel.h>

int	ui_whois(t_ui_panel *ui, const char *msg)
{
	ui_new_message(ui, msg, UI_INFO_MSG);
	return (0);
}

int	ui_names(t_ui_panel *ui, const char *msg)
{
	ui_new_message(ui, msg, UI_INFO_MSG);
	return (0);
}
