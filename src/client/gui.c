/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:20:22 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:22:32 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/login.h>
#include <client/ui/panel.h>

int	gui(t_env *e, int argc, char **argv)
{
	gtk_init(&argc, &argv);
	e->ui = (t_ui_login *)malloc(sizeof(t_ui_login));
	if (e->ui == NULL || ui_init_login(e, e->ui) < 0)
		return (-1);
	gtk_main();
	return (0);
}
