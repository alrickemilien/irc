/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_who.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:11:12 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:11:15 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** In the absence of the <name> parameter, all visible
** (users who aren't invisible (user mode +i) and who don't have a
** common channel with the requesting client) are listed.
** The same result can be achieved by using a <name> of "0" or any wildcard
** which
** will end up matching every entry possible.
*/

int			irc_who_all(t_env *e, int cs, t_token *to)
{
	size_t	i;

	i = 0;
	while (i <= e->max)
	{
		if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1 &&
				strcmp(e->channels[e->fds[cs].channel].channel,
					e->channels[e->fds[i].channel].channel) != 0)
		{
			irc_reply(e, cs, RPL_WHOREPLY,
					e->channels[e->fds[i].channel].channel,
					e->fds[i].username, "server", e->fds[i].host,
					e->fds[i].nickname, e->fds[i].realname);
			irc_reply(e, cs, RPL_ENDOFWHO, to[1].addr);
		}
		i++;
	}
	return (IRC_WHO);
}

static int	irc_who_is_all(const char *str, size_t len)
{
	return (!str || (len == 1 && strncmp(str, "*", 1)) ||
		(len == 1 && strncmp(str, "0", 1)));
}

int			irc_who(t_env *e, int cs, t_token *to)
{
	size_t	i;

	if (irc_who_is_all(to[1].addr, to[1].len))
		return (irc_who_all(e, cs, to));
	i = 0;
	while (i <= e->max)
	{
		if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1)
		{
			if (strncmp(e->fds[i].host, to[1].addr, to[1].len) == 0
			|| strncmp(e->fds[i].realname, to[1].addr, to[1].len) == 0 ||
				strncmp(e->fds[i].nickname, to[1].addr, to[1].len) == 0)
			{
				irc_reply(e, cs, RPL_WHOREPLY,
						e->channels[e->fds[i].channel].channel,
						e->fds[i].username, "server", e->fds[i].host,
						e->fds[i].nickname, e->fds[i].realname);
				irc_reply(e, cs, RPL_ENDOFWHO, to[1].addr);
				return (IRC_WHO);
			}
		}
		i++;
	}
	irc_reply(e, cs, ERR_NOSUCHNICK, to[1].addr);
	return (IRC_WHO);
}
