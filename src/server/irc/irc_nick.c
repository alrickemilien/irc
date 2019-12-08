/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_nick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:51:02 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:51:02 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static int	irc_nick_check_command(t_env *e, int cs, const t_token *tokens)
{
	const char	*nick;
	size_t		nick_len;
	size_t		i;

	if (!tokens[1].addr || tokens[2].addr)
		return (irc_err(e, cs, ERR_NONICKNAMEGIVEN, NULL));
	nick = tokens[1].addr;
	nick_len = tokens[1].len;
	if (nick_len > 9 || !nick_len || !is_valid_nick(nick))
		return (irc_err(e, cs, ERR_ERRONEUSNICKNAME, nick));
	i = 0;
	while (i <= e->max)
	{
		if (i != (size_t)cs && e->fds[i].type == FD_CLIENT &&
				strcmp(e->fds[i].nickname, nick) == 0)
			return (irc_err(e, cs, ERR_NICKNAMEINUSE, nick));
		i++;
	}
	return (0);
}

int			irc_nick(t_env *e, int cs, t_token *tokens)
{
	char	concat[512];

	if ((irc_nick_check_command(e, cs, tokens)) != 0)
		return (-1);
	memset(concat, 0, sizeof(concat));
	if (e->fds[cs].registered)
	{
		sprintf(concat, ":%s NICK %s", e->fds[cs].nickname, tokens[1].addr);
		broadcast_all(e, concat, IRC_NOTICE, cs);
		logdebug("%s\n", concat);
		return (IRC_NICK);
	}
	memrpl(e->fds[cs].nickname, NICKNAMESTRSIZE, tokens[1].addr, tokens[1].len);
	if (e->fds[cs].username[0] == 0)
		return (IRC_NICK);
	e->fds[cs].registered = 1;
	irc_reply(e, cs, RPL_WELCOME, e->fds[cs].username, e->fds[cs].host,
			e->fds[cs].realname);
	irc_user_join_default_channel(e, cs);
	return (IRC_NICK);
}
