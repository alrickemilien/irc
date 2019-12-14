/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_rpl_topic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:12:10 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:14:01 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

int	s2c_rpl_topic(t_env *e, t_token *tokens)
{
	(void)e;
	if (!tokens[1].addr || !tokens[2].addr)
		return (-1);
	loginfo("TOPIC: %s",
			tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr);
	if (e->options.gui)
		ui_new_message(
				e->ui,
				tokens[2].addr[0] == ':' ? tokens[2].addr + 1 : tokens[2].addr,
				UI_TOPIC_MSG);
	return (IRC_S2C_RPL_TOPIC);
}

int	s2c_rpl_notopic(t_env *e, t_token *tokens)
{
	(void)e;
	if (!tokens[1].addr)
		return (-1);
	loginfo("%s", tokens[1].addr);
	if (e->options.gui)
		ui_new_message(
				e->ui,
				tokens[1].addr,
				UI_TOPIC_MSG);
	return (IRC_S2C_RPL_NOTOPIC);
}
