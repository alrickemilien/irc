/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:24:18 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 11:42:38 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cbuffer/cbuffer_ssl.h>
#include <client/irc.h>
#include <client/ui/panel.h>

void		disconnect(t_env *e, int cs)
{
	if (e->options.gui)
		ui_set_status(e->ui, 1);
	close(cs);
	clear_fd(&e->fds[cs]);
	logerror("Connection with server has been lost");
	e->sock = -1;
	FD_CLR(cs, &e->fd_read);
	FD_CLR(cs, &e->fd_write);
}

static int	flush_read_buffer(t_env *e, size_t cs, size_t index)
{
	t_fd	*fd;
	char	command[512];

	(void)cs;
	fd = e->self;
	while (index != (size_t)-1)
	{
		memset(command, 0, CBUFFSIZE);
		if (fd->buf_read.tail < index)
		{
			memcpy(command, fd->buf_read.buffer + fd->buf_read.tail,
					index - fd->buf_read.tail);
		}
		else
		{
			memcpy(command, fd->buf_read.buffer + fd->buf_read.tail,
					CBUFFSIZE - fd->buf_read.tail);
			memcpy(command + CBUFFSIZE - fd->buf_read.tail, fd->buf_read.buffer,
					index);
		}
		s2c(e, command);
		cbuffer_drop_until(&fd->buf_read, "\x0D\x0A");
		index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
	}
	return (0);
}

int			server_read(t_env *e, size_t cs)
{
	size_t	r;
	size_t	index;
	t_fd	*fd;

	fd = e->self;
	r = e->options.ssl ? cbuffer_read_ssl(&fd->buf_read, fd->ssl)
		: cbuffer_recv(&fd->buf_read, cs);
	if (r <= 0)
	{
		disconnect(e, cs);
		return (r);
	}
	index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
	if (index == (size_t)-1)
	{
		if (cbuffer_size(&fd->buf_read) >= CBUFFSIZE)
		{
			logerror("[!] Buffer is reset because it is full without command");
			cbuffer_reset(&fd->buf_read);
		}
		return (0);
	}
	return (flush_read_buffer(e, cs, index));
}
