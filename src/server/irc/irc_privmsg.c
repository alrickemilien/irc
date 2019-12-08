/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_privmsg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:17:40 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:17:41 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static int	irc_privmsg_check_command(
		t_env *e, int cs, const t_token *tokens)
{
	if (!tokens[1].addr || !tokens[1].len)
		return (irc_err(e, cs, ERR_NOSUCHNICK, NULL));
	if (!tokens[2].addr)
		return (irc_err(e, cs, ERR_NOTEXTTOSEND, NULL));
	if (tokens[2].addr[0] == ':' &&
		(tokens[2].addr[1] == 0 ||
		tokens[2].addr + 1 == strstr(tokens[2].addr, "\x0D\x0A")))
		return (irc_err(e, cs, ERR_NOTEXTTOSEND, NULL));
	return (0);
}

static int	irc_privmsg_to_client(
		t_fd *fd, t_fd *fd_client, const char *msg)
{
	cbuffer_putstr(&fd_client->buf_write, ":");
	cbuffer_putstr(&fd_client->buf_write, fd->nickname);
	cbuffer_putstr(&fd_client->buf_write, " PRIVMSG :");
	cbuffer_putstr(&fd_client->buf_write, msg[0] == ':' ? msg + 1 : msg);
	cbuffer_putstr(&fd_client->buf_write, "\x0D\x0A");
	return (0);
}

void		irc_privmsg_nomatch_nick(
		t_env *e,
		int cs,
		t_token *subtokens,
		size_t subtoken_count)
{
	size_t j;

	j = 0;
	while (j < subtoken_count)
	{
		if (subtokens[j].addr != NULL && subtokens[j].addr[0] != '&' &&
				subtokens[j].addr[0] != '#')
			irc_err(e, cs, ERR_NOSUCHNICK, subtokens[j].addr);
		j++;
	}
}

static int	is_nick_or_chan_matching(t_env *e,
	size_t i,
	const char *src,
	size_t src_len)
{
	const char *nick;
	const char *channel;

	nick = e->fds[i].nickname;
	channel = e->channels[e->fds[i].channel].channel;
	return (src && (strncmp(nick, src, src_len) == 0 ||
				strncmp(channel, src, src_len) == 0));
}

/*
** index[0] => i
** index[1] => j
** t[0] => tokens
** t[1] => subtokens
*/

static int	norme_42_sucks(t_env *e, int cs, size_t *index, t_token **t)
{
	if (e->fds[index[0]].away && t[1][index[1]].addr[0] != '&' && t[1][index[1]].addr[0] != '#')
		irc_reply(e, cs, RPL_AWAY, e->fds[index[0]].nickname, e->fds[index[0]].awaymessage);
	else
		irc_privmsg_to_client(&e->fds[cs], &e->fds[index[0]], t[0][2].addr);
	if (t[1][index[1]].addr[0] != '&' && t[1][index[1]].addr[0] != '#')
		t[1][index[1]].addr = (void *)0;
	return (0);
}

int			irc_privmsg(t_env *e, int cs, t_token *tokens)
{
	size_t	i;
	size_t	j;
	size_t	subtoken_count;
	t_token	sub[30];

	if ((irc_privmsg_check_command(e, cs, tokens)) != 0)
		return (-1);
	subtoken_count = tokenizechr(tokens[1].addr, sub, 30, ',');
	i = 0;
	while (i <= e->max)
	{
		j = 0;
		while (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1 && j < subtoken_count)
		{
			if (is_nick_or_chan_matching(e, i, sub[j].addr, sub[j].len)
				&& !norme_42_sucks(e, cs, (size_t[]){i, j}, (t_token*[]){tokens, sub}))
				break ;
			j++;
		}
		i++;
	}
	irc_privmsg_nomatch_nick(e, cs, sub, subtoken_count);
	return (IRC_PRIVMSG);
}
