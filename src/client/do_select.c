/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:19:46 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:20:05 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <client/irc.h>

void	clear_fd(t_fd *fd)
{
	fd->registered = 0;
	fd->type = FD_FREE;
	fd->read = NULL;
	fd->write = NULL;
}

void	init_fd(t_env *e)
{
	size_t i;

	e->max = 0;
	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);
	i = 0;
	while (i < e->maxfd)
	{
		if (e->fds[i].type != FD_FREE)
		{
			FD_SET(i, &e->fd_read);
			if (cbuffer_size(&e->fds[i].buf_write) > 0)
				FD_SET(i, &e->fd_write);
			e->max = e->maxfd > i ? e->maxfd : i;
		}
		i++;
	}
}

void	check_fd(t_env *e)
{
	size_t	i;

	i = 0;
	while ((i < e->maxfd) && (e->r > 0))
	{
		if (FD_ISSET(i, &e->fd_read))
			e->fds[i].read(e, i);
		if (FD_ISSET(i, &e->fd_write))
			e->fds[i].write(e, i);
		if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
			e->r--;
		i++;
	}
}

int		do_select(t_env *e)
{
	struct timeval	timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 500;
	init_fd(e);
	if ((e->r = select(e->maxfd, &e->fd_read,
		&e->fd_write, (void *)0, &timeout)) < 0)
		return (logerrno("do_select::select"));
	check_fd(e);
	return (0);
}
