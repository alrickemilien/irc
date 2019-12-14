/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2s_nick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:41:44 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:41:45 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>
#include <client/ui/panel.h>

static int	c2s_nick_check_command(
	t_env *e, const t_token *tokens)
{
	if (!tokens[1].addr)
		return (irc_error(e, ERR_NONICKNAMEGIVEN));
	if (tokens[2].addr)
		return (irc_error(e, ERR_NICK_BAD_FORMAT));
	if (tokens[1].len > 9 || !tokens[1].len || !is_valid_nick(tokens[1].addr))
		return (irc_error(e, ERR_ERRONEUSNICKNAME, tokens[1].addr));
	return (0);
}

int			do_c2s_nick(
	t_env *e, const char *nick, size_t nick_length)
{
	if (e->sock == -1)
	{
		memrpl(e->nick, NICKNAMESTRSIZE, nick, nick_length);
		return (0);
	}
	if (cbuffer_putcmd(&e->self->buf_write, "NICK %.*s\x0D\x0A",
				nick_length, nick) < 0)
		return (-1);
	memrpl(e->self->nickname, NICKNAMESTRSIZE, nick, nick_length);
	return (0);
}

int			c2s_nick(t_env *e, t_token *tokens)
{
	if ((c2s_nick_check_command(e, tokens)) < 0)
		return (-1);
	do_c2s_nick(e, tokens[1].addr, tokens[1].len);
	if (e->options.gui)
		ui_set_nick(e->ui, tokens[1].addr);
	return (IRC_C2S_NICK);
}
