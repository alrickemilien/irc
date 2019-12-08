/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_user.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:04:29 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:04:30 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** Command: USER
** Parameters: <username> <hostname> <servername> <realname>
**
** Between servers USER must to be prefixed with client's NICKname.
** Note that hostname and servername are normally ignored by the IRC
** server when the USER command comes from a directly connected client,
** but they are used in server to server
** communication.
*/

static int	irc_user_check_command(
	t_env *e, int cs, const t_token *tokens)
{
	size_t	i;
	char	*username;
	size_t	username_len;

	if (!tokens[1].addr || !tokens[2].addr || !tokens[3].addr ||
			!tokens[4].addr)
		return (irc_err(e, cs, ERR_NEEDMOREPARAMS, "USER"));
	username = tokens[1].addr;
	username_len = tokens[1].len;
	i = 0;
	while (i <= e->max)
	{
		if (e->fds[i].type == FD_CLIENT &&
				strncmp(e->fds[i].username, username, username_len) == 0)
			return (irc_err(e, cs, ERR_ALREADYREGISTRED, NULL));
		i++;
	}
	return (0);
}

void		irc_user_join_default_channel(t_env *e, int cs)
{
	char concat[512];

	e->fds[cs].channel = 0;
	e->channels[0].clients++;
	memset(concat, 0, sizeof(concat));
	sprintf(concat, "%s!%s@%s JOIN %s\x0D\x0A", e->fds[cs].nickname,
			e->fds[cs].username, e->fds[cs].host,
			e->channels[0].channel);
	cbuffer_putstr(&e->fds[cs].buf_write, concat);
}

int			irc_user(t_env *e, int cs, t_token *tokens)
{
	if ((irc_user_check_command(e, cs, tokens)) < 0)
		return (-1);
	memrpl(e->fds[cs].username, USERNAMESTRSIZE, tokens[1].addr,
			tokens[1].len < USERNAMESTRSIZE ? tokens[1].len : USERNAMESTRSIZE);
	memrpl(e->fds[cs].host, HOSTNAMESTRSIZE, tokens[2].addr,
			tokens[2].len < HOSTNAMESTRSIZE ? tokens[2].len : HOSTNAMESTRSIZE);
	memrpl(e->fds[cs].realname, USERNAMESTRSIZE,
			tokens[4].addr[0] == ':' ? tokens[4].addr + 1 : tokens[4].addr,
			tokens[4].addr[0] == ':' ? tokens[4].len - 1 : tokens[4].len);
	if (e->fds[cs].nickname[0] == 0)
	{
		logdebug(
			"USER command received for client #%ld without nickname, do not "
			"set as registered now\n",
			cs);
		return (IRC_USER);
	}
	e->fds[cs].registered = 1;
	irc_reply(e, cs, RPL_WELCOME, e->fds[cs].username, e->fds[cs].host,
			e->fds[cs].realname);
	irc_user_join_default_channel(e, cs);
	return (IRC_USER);
}
