/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s2c_invite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 13:22:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 13:22:41 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

int	s2c_invite(t_env *e, t_token *tokens)
{
	char	msg[512];

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "%.*s invites you to channel %.*s",
		(int)tokens[0].len - 1,
		tokens[0].addr + 1, (int)tokens[3].len, tokens[3].addr);
	loginfo(msg);
	if (e->options.gui)
		ui_new_message(e->ui, msg, UI_INVITE_MSG);
	return (IRC_S2C_RPL_INVITING);
}
