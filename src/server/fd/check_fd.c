/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:19:16 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 17:19:17 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

/*
** Loop over file descriptors
*/

void	check_fd(t_env *e)
{
	size_t				i;
	const unsigned long	t = (unsigned long)time(NULL);

	i = 0;
	while (i < e->maxfd)
	{
		if (FD_ISSET(i, &e->fd_read))
		{
			e->fds[i].read(e, i);
			e->fds[i].la = t;
		}
		if (FD_ISSET(i, &e->fd_write))
			e->fds[i].write(e, i);
		if (e->fds[i].la != 0 &&
			(t - e->fds[i].la) >= TOLERATED_INACTIVITY_TIME_S &&
			(e->fds[i].last_ping_activity == 0 ||
			(t - e->fds[i].last_ping_activity) >= PING_INTERVAL_S))
		{
			ping(e, i);
			e->fds[i].last_ping_activity = t;
		}
		if (e->fds[i].la != 0 && (t - e->fds[i].la) >= MTITS)
			disconnect(e, i);
		i++;
	}
}
