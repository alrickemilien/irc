/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2s_away.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:43:15 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:43:16 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

int	do_c2s_away(
		t_fd *fd, const char *msg, size_t msg_len)
{
	return (cbuffer_putcmd(&fd->buf_write, "AWAY %.*s\x0D\x0A",
		msg_len < AWAYMSGSIZE ? msg_len : AWAYMSGSIZE, msg));
}

int	c2s_away(t_env *e, t_token *tokens)
{
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	if (tokens[1].addr == (void *)0)
		return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));
	do_c2s_away(e->self, tokens[1].addr, strlen(tokens[1].addr));
	return (IRC_C2S_AWAY);
}

int	do_c2s_unaway(t_fd *fd)
{
	return (cbuffer_putstr(&fd->buf_write, "AWAY\x0D\x0A"));
}

int	c2s_unaway(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	do_c2s_unaway(e->self);
	return (IRC_C2S_UNAWAY);
}
