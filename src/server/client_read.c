/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:33:26 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:35:44 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cbuffer/cbuffer_ssl.h>
#include <server/irc.h>

static int	empty_client_read_buffer(t_env *e, size_t cs, size_t index)
{
	t_fd	*fd;

	fd = &e->fds[cs];
	while (index != (size_t)-1)
	{
		if (irc_command(e, cs, index) == IRC_QUIT)
		{
			disconnect(e, cs);
			return (0);
		}
		cbuffer_drop_until(&fd->buf_read, "\x0D\x0A");
		index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
	}
	return (0);
}

/*
** Data available on read on the socket cs
*/

int			client_read(t_env *e, size_t cs)
{
	size_t	r;
	size_t	index;
	t_fd	*fd;

	fd = &e->fds[cs];
	index = -1;
	if (cbuffer_size(&fd->buf_read) != CBUFFSIZE &&
		(cbuffer_isempty(&fd->buf_read) ||
		(index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A")) == (size_t)-1))
	{
		r = e->ssl_ctx ? cbuffer_read_ssl(&fd->buf_read, fd->ssl)
			: cbuffer_recv(&fd->buf_read, cs);
		if (r <= 0)
		{
			disconnect(e, cs);
			return (0);
		}
	}
	if (index == (size_t)-1)
		index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
	if (index == (size_t)-1 && fd->buf_read.full)
		cbuffer_reset(&fd->buf_read);
	if (index == (size_t)-1)
		return (0);
	return (empty_client_read_buffer(e, cs, index));
}
