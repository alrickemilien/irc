/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_ping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 15:56:08 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:56:10 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int	irc_ping(t_env *e, int cs, t_token *tokens)
{
	(void)tokens;
	cbuffer_putstr(&e->fds[cs].buf_write, "PONG\x0D\x0A");
	return (IRC_PING);
}
