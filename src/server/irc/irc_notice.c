/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_notice.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:15:13 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:15:38 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static int	irc_notice_check_command(
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

static int	matchin(t_env *e,
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

/*
** size_t *idx stands for idx[0] and idx[1]
** idx[0] => i
** idx[1] => j
** t_tokens **t stands for t[0] and t[1]
** t[0] => tokens
** t[1] => sub
*/

int			inr(t_env *e, int cs, t_token **t, size_t *idx)
{
	if (matchin(e, idx[0], t[1][idx[1]].addr,
			t[1][idx[1]].len) && !e->fds[idx[0]].away)
	{
		irc_privmsg_to_client(&e->fds[cs], &e->fds[idx[0]],
				t[0][2].addr);
		if (t[1][idx[1]].addr[0] != '&'
			&& t[1][idx[1]].addr[0] != '#')
			t[1][idx[1]].addr = (void *)0;
		return (1);
	}
	return (0);
}

int			irc_notice(t_env *e, int cs, t_token *t)
{
	int		i;
	size_t	j;
	size_t	subtoken_count;
	t_token	sub[30];

	if ((irc_notice_check_command(e, cs, t)) < 0)
		return (-1);
	subtoken_count = tokenizechr(t[1].addr, sub, 30, ',');
	i = 0;
	while (i <= (int)e->max)
	{
		if (i != cs && e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1)
		{
			j = 0;
			while (j < subtoken_count)
			{
				if (inr(e, cs, (t_token*[]){t, sub}, (size_t[]){(int)i, j}))
					break ;
				j++;
			}
		}
		i++;
	}
	irc_privmsg_nomatch_nick(e, cs, sub, subtoken_count);
	return (IRC_PRIVMSG);
}
