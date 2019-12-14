/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_rpl_version.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:11:37 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:11:37 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

int	s2c_rpl_version(t_env *e, t_token *tokens)
{
	loginfo("%s", tokens[1].addr);
	if (e->options.gui)
		ui_new_message(e->ui, tokens[1].addr, UI_INFO_MSG);
	return (IRC_S2C_RPL_VERSION);
}
