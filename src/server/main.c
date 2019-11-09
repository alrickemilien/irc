#include <stdlib.h>
#include <sys/resource.h>
#include "server/irc.h"

void init_env(t_env *e)
{
    size_t        i;
    struct rlimit rlp;

    XSAFE(-1, getrlimit(RLIMIT_NOFILE, &rlp), "init_env::getrlimit");

    e->maxfd = rlp.rlim_cur;
    e->fds = (t_fd *)XPSAFE(NULL, malloc(sizeof(*e->fds) * e->maxfd), "init_env::malloc");

    i = 0;
    while (i < e->maxfd)
        clear_fd(&e->fds[i++]);
}

static void init_options(t_options *options)
{
    // Set default port
    if (options->port == 0)
        options->port = 5555;

    // Set default backlog
    if (options->backlog == 0)
        options->backlog = 42;

    if (options->daemon)
        daemonize();

    // Set default backlog
    if (options->bind[0] == 0)
        memcpy(options->bind, "127.0.0.1", sizeof(char) * 9);

    if (options->ipv6) {
        printf("Running server ipv6\n");
    }
}

int main(int argc, const char **argv)
{
    int       exit_code;
    t_options options;
    t_env     e;

    exit_code = read_options_arguments(argc, argv, &options);
    if (exit_code != 0)
        return (exit_code);

    init_options(&options);

    e.is_tty = isatty(1);

    printf("Running at %s:%d\n", options.bind, options.port);

    init_env(&e);

    if (options.ipv6 == 1)
        server_ipv6(&options, &e);
    else
        server_ipv4(&options, &e);

    serv(&e);

    return (exit_code);
}
