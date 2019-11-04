#include "irc.h"
#include <stdlib.h>
#include <sys/resource.h>

void init_env(t_env *e)
{
    size_t        i;
    struct rlimit rlp;

    XSAFE(-1, getrlimit(RLIMIT_NOFILE, &rlp), "init_env::getrlimit");

    e->maxfd = rlp.rlim_cur;
    e->fds = (t_fd *)XPSAFE(NULL, malloc(sizeof(*e->fds) * e->maxfd), "malloc");

    i = 0;
    while (i < e->maxfd)
    {
        clear_fd(&e->fds[i]);
        i++;
    }
}

int main(int argc, const char **argv)
{
	int			exit_code;
	t_options	options;
    t_env e;

	exit_code = read_options_arguments(argc, argv, &options);
    if (exit_code != 0)
		return (exit_code);

    // Set default port
    if (options.port == 0)
        options.port = 5555;

    init_env(&e);
    server_ipv4(&e, options.port);
    server_ipv6(&e, options.port + 1);
    serv(&e);

	return (exit_code);
}
