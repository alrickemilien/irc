/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 14:09:37 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 14:09:37 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

static int	flush_stdin_read_buffer(t_env *e, size_t cs, size_t index)
{
	char	command[512];

	while (index != (size_t)-1)
	{
		memset(command, 0, CBUFFSIZE);
		if (e->fds[cs].buf_read.tail < index)
			memcpy(command,
				e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
				index - e->fds[cs].buf_read.tail);
		else
		{
			memcpy(command,
				e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
				CBUFFSIZE - e->fds[cs].buf_read.tail);
			memcpy(command + CBUFFSIZE - e->fds[cs].buf_read.tail,
				e->fds[cs].buf_read.buffer, index);
		}
		c2s(e, command);
		cbuffer_dropn(&e->fds[cs].buf_read,
			(e->fds[cs].buf_read.tail < index
			? index - e->fds[cs].buf_read.tail
			: index + CBUFFSIZE - e->fds[cs].buf_read.tail) + 1);
		index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A");
	}
	return (0);
}

int			stdin_read(t_env *e, size_t cs)
{
	size_t	r;
	size_t	index;

	index = -1;
	if (cbuffer_size(&e->fds[cs].buf_read) != CBUFFSIZE &&
		(cbuffer_isempty(&e->fds[cs].buf_read) ||
		(index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A")) == (size_t)-1))
	{
		r = cbuffer_read(&e->fds[cs].buf_read, cs);
		if (r <= 0)
			return (r);
	}
	if (index == (size_t)-1)
		index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A");
	if (index == (size_t)-1)
	{
		if (cbuffer_size(&e->fds[cs].buf_read) >= CBUFFSIZE)
		{
			logerror("[!] Buffer is reset because it is full without command");
			cbuffer_reset(&e->fds[cs].buf_read);
		}
		return (0);
	}
	return (flush_stdin_read_buffer(e, cs, index));
}
