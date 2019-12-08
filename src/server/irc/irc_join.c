/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:44:47 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:44:52 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** the only restriction on a
** channel name is that it may not contain any spaces (' '), a control G
** (^G or ASCII 7), or a comma (',' which is used as a list item
** separator by the protocol)
*/

static int	irc_join_check_command(t_env *e, int cs, const t_token *to)
{
	const char	*channel;
	size_t		channel_len;

	if (!to[1].addr || to[2].addr)
		return (irc_err(e, cs, ERR_NEEDMOREPARAMS, NULL));
	channel = to[1].addr;
	channel_len = to[1].len;
	if (channel_len - 1 > CHANNELSTRSIZE)
		return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
	else if (!is_valid_chan_name(channel, channel_len))
		return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
	else if (channel_len < 1)
		return (irc_err(e, cs, ERR_NOSUCHCHANNEL, channel));
	return (0);
}

size_t		find_chan_index(t_env *e, t_token *to)
{
	size_t	i;
	size_t	empty_channel;

	i = 0;
	empty_channel = 0;
	while (i < e->maxchannels)
	{
		if (strncmp(e->channels[i].channel, to[1].addr, to[1].len) == 0)
			break ;
		if (empty_channel == 0 && e->channels[i].channel[0] == 0)
			empty_channel = i;
		i++;
	}
	if (i == e->maxchannels)
		i = empty_channel;
	return (i);
}

void		irc_join_broadcast(t_env *e, int cs)
{
	char	concat[CHANNELSTRSIZE + NICKNAMESTRSIZE + 11];

	memset(concat, 0, sizeof(concat));
	sprintf(concat, "%s!%s@%s JOIN %s\x0D\x0A", e->fds[cs].nickname,
		e->fds[cs].username,
		e->fds[cs].host,
		e->channels[e->fds[cs].channel].channel);
	broadcast_all_in_channel(e, concat, IRC_NOTICE, cs);
	loginfo("%s!%s@%s JOIN %s\n", e->fds[cs].nickname, e->fds[cs].nickname,
			e->fds[cs].host, e->channels[e->fds[cs].channel].channel);
}

int			irc_join(t_env *e, int cs, t_token *to)
{
	size_t	i;

	if ((irc_join_check_command(e, cs, to)) != 0)
		return (-1);
	i = find_chan_index(e, to);
	if (i == e->fds[cs].channel)
		return (IRC_JOIN);
	memrpl(e->channels[i].channel, CHANNELSTRSIZE + 1, to[1].addr, to[1].len);
	e->channels[e->fds[cs].channel].clients--;
	if (e->fds[cs].channel != 0 && e->channels[e->fds[cs].channel].clients == 0)
		memset(&e->channels[e->fds[cs].channel], 0, sizeof(t_channel));
	e->fds[cs].channel = i;
	irc_join_broadcast(e, cs);
	if (e->channels[e->fds[cs].channel].topic[0])
		irc_reply(e, cs, RPL_TOPIC, e->channels[e->fds[cs].channel].channel,
				e->channels[e->fds[cs].channel].topic);
	else
		irc_reply(e, cs, RPL_NOTOPIC, e->channels[e->fds[cs].channel].channel);
	if (e->channels[i].clients == 0)
		e->channels[i].chop = cs;
	e->channels[i].clients++;
	return (IRC_JOIN);
}
