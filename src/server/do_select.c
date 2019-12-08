/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:38:42 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:38:52 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** select returns the number of file descriptors
** contained in the three returned descriptor sets
*/

int	do_select(t_env *e)
{
	struct timeval	timeout;

	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	while (1)
	{
		init_fd(e);
		if ((e->r = select(e->max + 1, &e->fd_read, &e->fd_write, (void *)0,
			&timeout)) < 0)
			return (logerrno("do_select::select"));
		check_fd(e);
	}
	return (0);
}
