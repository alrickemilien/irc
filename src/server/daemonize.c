/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:27:02 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:27:25 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <server/irc.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

static int	write_pidfile(pid_t pid)
{
	int		fd;
	char	pidstr[100];
	size_t	pidstr_len;

	if ((fd = open(PIDFILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG)) <
			0)
		return (logerrno("write_pidfile::open"));
	if ((pidstr_len = i64toa(pid, pidstr, 100, 10)) == (size_t)-1)
		return (logerror("write_pidfile::i64toa"));
	if (write(fd, pidstr, pidstr_len) < 0)
		return (logerrno("write_pidfile::write"));
	if (close(fd) < 0)
		return (logerrno("write_pidfile::close"));
	return (0);
}

int			daemonize(void)
{
	pid_t	pid;
	int		x;

	if (write_pidfile(0) < 0)
		return (-1);
	if ((pid = fork()) < 0)
		return (logerrno("daemonize::fork"));
	if (pid > 0)
		exit(0);
	if (setsid() < 0)
		return (logerrno("daemonize::setsid"));
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	if ((pid = fork()) < 0)
		return (logerrno("daemonize::fork"));
	if (pid > 0)
	{
		write_pidfile(pid);
		exit(0);
	}
	umask(0);
	chdir("/");
	x = sysconf(_SC_OPEN_MAX);
	while (x >= 0)
		close(x--);
	openlog("irc_daemon", LOG_PID, LOG_DAEMON);
	return (0);
}
