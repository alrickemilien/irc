/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:47:13 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:47:14 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int	irc_list_all(t_env *e, int cs)
{
	size_t	i;

	i = 0;
	while (i < e->maxchannels)
	{
		if (i == 0 ||
				(e->channels[i].channel[0] && e->channels[i].clients > 0))
			irc_reply(e, cs, RPL_LIST,
				e->channels[i].channel,
				e->channels[i].topic);
		i++;
	}
	irc_reply(e, cs, RPL_LISTEND, NULL);
	return (IRC_LIST);
}

int	irc_list(t_env *e, int cs, t_token *tokens)
{
	size_t	i;
	size_t	j;
	size_t	subtoken_count;
	t_token	subtokens[30];

	irc_reply(e, cs, RPL_LISTSTART, NULL);
	if (!tokens[1].addr)
		return (irc_list_all(e, cs));
	subtoken_count = tokenizechr(tokens[1].addr, subtokens, 30, ',');
	i = 0;
	while (i < e->maxchannels)
	{
		j = 0;
		while (j < subtoken_count)
		{
			if (strncmp(e->channels[i].channel,
						subtokens[j].addr, subtokens[j].len) == 0)
				irc_reply(e, cs, RPL_LIST, e->channels[i].channel,
						e->channels[i].topic);
			j++;
		}
		i++;
	}
	irc_reply(e, cs, RPL_LISTEND, NULL);
	return (IRC_LIST);
}
