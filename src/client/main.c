/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 16:22:16 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 16:22:16 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int			init_env(t_env *e)
{
	size_t			i;
	struct rlimit	rlp;

	if (init_i18n(e->argv_0) < 0)
		return (-1);
	if (getrlimit(RLIMIT_NOFILE, &rlp) < 0)
		return (logerrno("init_env::getrlimit"));
	e->maxfd = rlp.rlim_cur;
	if ((e->fds = (t_fd *)malloc(sizeof(*e->fds) * e->maxfd)) == (void *)0)
		return (logerror("init_env::malloc"));
	e->sock = -1;
	if (getcwd(e->cwd, sizeof(e->cwd)) == (void *)0)
		return (logerrno("init_env::getcwd"));
	i = 0;
	while (i < e->maxfd)
	{
		memset(&e->fds[i], 0, sizeof(t_fd));
		clear_fd(&e->fds[i]);
		i++;
	}
	return (0);
}

static void	init_std(t_env *e)
{
	t_fd	*stdin_fd;

	stdin_fd = &e->fds[0];
	stdin_fd->type = FD_CLIENT;
	stdin_fd->read = stdin_read;
}

static void	init_connection(t_env *e)
{
	if (e->options.port == 0)
		e->options.port = IRC_DEFAULT_SERVER_PORT;
	if (e->options.host[0] != 0)
		do_c2s_connect(e, NULL, NULL, NULL);
}

static void	execute_precommands(t_env *e)
{
	char	*ptr;

	ptr = e->options.command;
	while (ptr && *ptr)
	{
		c2s(e, ptr);
		ptr = strstr(ptr, "\x0A");
		if (ptr)
			ptr += 1;
	}
}

int			main(int argc, char **argv)
{
	int		exit_code;
	t_env	e;

	memset(&e, 0, sizeof(t_env));
	exit_code = read_options(argc, (const char **)argv, &e.options);
	if (exit_code != 0)
		return (exit_code);
	e.argv_0 = argv[0];
	init_env(&e);
	if (e.options.ipv6 == 1)
		e.ipv6 = 1;
	if (e.options.gui)
		return (gui(&e, argc, argv));
	init_connection(&e);
	init_std(&e);
	execute_precommands(&e);
	while (1)
		do_select(&e);
	return (exit_code);
}
