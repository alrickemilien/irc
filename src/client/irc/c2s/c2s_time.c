/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2s_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:41:36 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:41:37 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

int	c2s_time(t_env *e, t_token *tokens)
{
	(void)tokens;
	if (e->sock == -1)
		return (irc_error(e, ERR_NOT_CONNECTED));
	cbuffer_putcmd(&e->self->buf_write, "TIME\x0D\x0A");
	return (IRC_C2S_TIME);
}
