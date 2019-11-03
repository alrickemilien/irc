#include "irc.h"
#include <stdlib.h>
#include <sys/resource.h>

void init_env(t_env *e)
{
    int           i;
    struct rlimit rlp;

    XSAFE(-1, getrlimit(RLIMIT_NOFILE, &rlp), "getrlimit");

    e->maxfd = rlp.rlim_cur;
    e->fds = (t_fd *)XPSAFE(NULL, malloc(sizeof(*e->fds) * e->maxfd), "malloc");

    i = 0;
    while (i < e->maxfd)
    {
        clear_fd(&e->fds[i]);
        i++;
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    t_env e;

    init_env(&e);

    server_create(&e, 5555);

    serv(&e);

    return (0);
}