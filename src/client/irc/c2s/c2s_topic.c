/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2s_topic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:44:58 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:44:59 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

static int	c2s_topic_check_command(
		t_env *e, const t_token *tokens)
{
	const char	*channel;
	size_t		channel_len;

	if (!tokens[1].addr)
		return (0);
	channel = tokens[1].addr;
	channel_len = tokens[1].len;
	if (channel_len - 1 > CHANNELSTRSIZE)
		return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
	else if (!is_valid_chan_name(channel, channel_len))
		return (irc_error(e, ERR_NOSUCHCHANNEL, tokens[1].addr));
	else if (channel_len < 1)
		return (irc_error(e, ERR_NOSUCHCHANNEL, ""));
	return (0);
}

int			do_c2s_topic(
		t_fd *fd, const char *chan,
		size_t chan_len, const char *msg)
{
	if (msg)
	{
		return (cbuffer_putcmd(&fd->buf_write,
				"TOPIC %.*s :%s\x0D\x0A", chan_len, chan, msg));
	}
	return (cbuffer_putcmd(&fd->buf_write, "TOPIC %.*s\x0D\x0A",
				chan_len, chan));
}

int			c2s_topic(t_env *e, t_token *tokens)
{
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	if (c2s_topic_check_command(e, tokens) < 0)
		return (-1);
	do_c2s_topic(e->self,
			tokens[1].addr ? tokens[1].addr : e->self->channelname,
			tokens[1].len ? tokens[1].len : strlen(e->self->channelname),
			tokens[2].addr ? tokens[2].addr : NULL);
	return (IRC_C2S_TOPIC);
}
