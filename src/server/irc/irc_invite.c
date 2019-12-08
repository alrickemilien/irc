/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_invite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:19:32 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:19:33 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static int	irc_invite_check_command(
		t_env *e, int cs, const t_token *to)
{
	const char	*cs_chan;

	if (!to[1].addr || !to[2].addr)
		return (irc_err(e, cs, ERR_NEEDMOREPARAMS, NULL));
	cs_chan = e->channels[e->fds[cs].channel].channel;
	if (!to[1].len)
		return (irc_err(e, cs, ERR_NOSUCHNICK, NULL));
	if (strncmp(cs_chan, to[2].addr, to[2].len) != 0 ||
			cs_chan[to[2].len] != 0)
		return (irc_err(e, cs, ERR_NOSUCHCHANNEL, cs_chan));
	else if (e->channels[e->fds[cs].channel].chop != cs)
		return (irc_err(e, cs, ERR_CHANOPRIVSNEEDED, cs_chan));
	return (0);
}

static void	l_irc_invite(
		t_env *e, int cs, size_t i, const char *cs_chan)
{
	cbuffer_putcmd(&e->fds[i].buf_write, ":%s INVITE %s %s\x0D\x0A",
		e->fds[cs].nickname, e->fds[i].nickname, cs_chan);
	irc_reply(e, cs, RPL_INVITING, e->fds[i].nickname, cs_chan,
		e->fds[i].nickname);
}

int			irc_invite(
		t_env *e, int cs, t_token *to)
{
	size_t		i;
	const char	*cs_chan = e->channels[e->fds[cs].channel].channel;
	const char	*i_chan;

	if ((irc_invite_check_command(e, cs, to)) != 0)
		return (-1);
	i = 0;
	while (i <= e->max)
	{
		if (strncmp(e->fds[i].nickname, to[1].addr, to[1].len) == 0)
		{
			i_chan = e->channels[e->fds[i].channel].channel;
			if (strncmp(i_chan, to[2].addr, to[2].len) == 0 &&
					i_chan[to[2].len] == 0)
				irc_reply(e, cs, ERR_USERONCHANNEL, e->fds[i].nickname, i_chan);
			else if (e->fds[i].away)
				irc_reply(e, cs, RPL_AWAY, e->fds[i].awaymessage);
			else
				l_irc_invite(e, cs, i, cs_chan);
			return (IRC_INVITE);
		}
		i++;
	}
	irc_reply(e, cs, ERR_NOSUCHNICK, e->fds[i].nickname);
	return (IRC_INVITE);
}
