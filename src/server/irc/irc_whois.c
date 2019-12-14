/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_whois.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 11:55:53 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 11:55:54 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

static int	irc_whois_check_command(
		t_env *e, int cs, const t_token *tokens)
{
	if (!tokens[1].addr)
		return (irc_err(e, cs, ERR_NONICKNAMEGIVEN, NULL));
	return (0);
}

int			irc_whois_nomatch_nick(
		t_env *e,
		int cs,
		t_token *subtokens,
		size_t subtoken_count)
{
	size_t j;

	j = 0;
	while (j < subtoken_count)
	{
		if (subtokens[j].addr != NULL)
			irc_err(e, cs, ERR_NOSUCHNICK, subtokens[j].addr);
		j++;
	}
	return (IRC_WHOIS);
}

/*
** size_t *idx stands for idx[0] and idx[1]
** idx[0] => i
** idx[1] => j
*/

int			irc_whois_reply(t_env *e, int cs, t_token *subtokens, size_t *idx)
{
	if (strncmp(e->fds[idx[0]].nickname, subtokens[idx[1]].addr,
				subtokens[idx[1]].len) == 0)
	{
		irc_reply(e, cs, RPL_WHOISUSER,
				e->fds[idx[0]].nickname,
				e->fds[idx[0]].username,
				e->fds[idx[0]].host,
				e->fds[idx[0]].realname);
		irc_reply(e, cs, RPL_WHOISCHANNELS,
				e->fds[idx[0]].nickname,
				e->channels[e->fds[idx[0]].channel].channel);
		irc_reply(e, cs, RPL_ENDOFWHOIS,
				e->fds[idx[0]].nickname);
		subtokens[idx[1]].addr = (void *)0;
		return (1);
	}
	return (0);
}

int			irc_whois(
		t_env *e, int cs, t_token *tokens)
{
	size_t	i;
	size_t	j;
	size_t	subtoken_count;
	t_token	subtokens[30];

	if (irc_whois_check_command(e, cs, tokens) < 0)
		return (-1);
	subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');
	i = 0;
	while (i <= e->max)
	{
		if (e->fds[i].type == FD_CLIENT && e->fds[i].registered == 1)
		{
			j = 0;
			while (subtokens[j].addr)
			{
				if (irc_whois_reply(e, cs, subtokens, (size_t[]){i, j}))
					break ;
				j++;
			}
		}
		i++;
	}
	return (irc_whois_nomatch_nick(e, cs, subtokens, subtoken_count));
}
