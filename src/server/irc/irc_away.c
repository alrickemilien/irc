/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_away.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:06:58 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:06:59 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int	irc_away(t_env *e, int cs, t_token *tokens)
{
	char	*away_msg;
	size_t	away_msg_len;

	if (!tokens[1].addr)
	{
		memset(e->fds[cs].awaymessage, 0, sizeof(AWAYMSGSIZE + 1));
		e->fds[cs].away = 0;
		irc_reply(e, cs, RPL_UNAWAY, NULL);
		loginfo("%s marked as not away", e->fds[cs].nickname);
		return (IRC_AWAY);
	}
	away_msg_len = tokens[1].addr[0] == ':' ? tokens[1].len - 1 : tokens[1].len;
	if (away_msg_len > AWAYMSGSIZE)
		away_msg_len = AWAYMSGSIZE;
	away_msg = tokens[1].addr[0] == ':' ? tokens[1].addr + 1 : tokens[1].addr;
	memrpl(e->fds[cs].awaymessage,
			sizeof(AWAYMSGSIZE + 1), away_msg, away_msg_len);
	e->fds[cs].away = 1;
	irc_reply(e, cs, RPL_NOWAWAY, NULL);
	loginfo("%s marked as away", e->fds[cs].nickname);
	return (IRC_AWAY);
}
