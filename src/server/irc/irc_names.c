/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_names.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:50:03 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:18:13 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** Check if buffer is full for new name
*/

static int	is_names_buf_full(
		const char *buf, size_t buf_size, const char *name)
{
	return ((buf_size - strlen(buf)) < (strlen(name) + 1));
}

static int	fill_names_buffer(t_env *e, int cs, size_t chan_index, int ret)
{
	char	concat[512];
	size_t	j;

	memset(concat, 0, sizeof(concat));
	j = 0;
	while (j <= e->max)
	{
		if (is_names_buf_full(concat, sizeof(concat), e->fds[j].nickname))
			break ;
		if (e->fds[j].type == FD_CLIENT && e->fds[j].registered &&
				e->fds[j].channel == chan_index)
		{
			strcat(concat, e->fds[j].nickname);
			strcat(concat, " ");
		}
		j++;
	}
	irc_reply(e, cs, RPL_NAMREPLY, e->channels[chan_index].channel, concat);
	irc_reply(e, cs, RPL_ENDOFNAMES, e->channels[chan_index].channel);
	if (ret == 0)
		return (strlen(concat));
	return (ret);
}

static int	irc_names_all_channels(t_env *e, int cs, t_token *tokens)
{
	size_t	i;

	(void)tokens;
	i = 0;
	while (i < e->maxchannels)
	{
		if (e->channels[i].channel[0])
			fill_names_buffer(e, cs, i, 0);
		i++;
	}
	return (IRC_NAMES);
}

static int	irc_names_one_channels(t_env *e, int cs, t_token *tokens)
{
	size_t	i;
	size_t	k;
	int		ret;

	i = 0;
	ret = 0;
	while (i < e->maxchannels)
	{
		k = 1;
		while (e->channels[i].channel[0] && tokens[k].addr)
		{
			if (strncmp(e->channels[i].channel, tokens[k].addr,
						tokens[k].len) == 0)
			{
				ret = fill_names_buffer(e, cs, i, ret);
				break ;
			}
			k++;
		}
		i++;
	}
	if (ret == 0)
		irc_reply(e, cs, RPL_ENDOFNAMES, NULL);
	return (IRC_NAMES);
}

int			irc_names(t_env *e, int cs, t_token *tokens)
{
	if (!tokens[1].addr)
		return (irc_names_all_channels(e, cs, tokens));
	return (irc_names_one_channels(e, cs, tokens));
}
