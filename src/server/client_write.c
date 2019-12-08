/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:36:29 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:37:00 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cbuffer/cbuffer_ssl.h>
#include <server/irc.h>

static int	empty_client_write_buffer(t_env *e, size_t cs, size_t index)
{
	t_fd	*fd;

	fd = &e->fds[cs];
	while (index != (size_t)-1)
	{
		if (!e->ssl_ctx &&
				cbuffer_send_until_str(cs, &fd->buf_write, "\x0D\x0A",
					MSG_NOSIGNAL) <= 0)
			return (-1);
		if (e->ssl_ctx &&
				cbuffer_write_ssl(fd->ssl, &fd->buf_write,
				(fd->buf_write.tail <= index
				? index - fd->buf_write.tail
				: index + CBUFFSIZE - fd->buf_write.tail) +
				2) <= 0)
			return (-1);
		index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");
	}
	return (0);
}

int			client_write(t_env *e, size_t cs)
{
	size_t	index;
	t_fd	*fd;

	fd = &e->fds[cs];
	if (fd->type != FD_CLIENT)
		return (0);
	index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");
	if ((index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A")) == (size_t)-1)
	{
		if (fd->buf_write.full)
		{
			logerror("[!] Buffer is reset because it is full without command");
			cbuffer_reset(&fd->buf_write);
		}
		return (0);
	}
	return (empty_client_write_buffer(e, cs, index));
}
