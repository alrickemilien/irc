#include <server/irc.h>

int do_select(t_env *e)
{
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;

    (void)timeout;

    while (1)
    {
        init_fd(e);

        // return the number of file descriptors
        // contained in the three returned descriptor sets
        if ((e->r = select(e->max + 1, &e->fd_read, &e->fd_write, (void *)0,
                           (void *)0)) < 0)
            return (logerrno("do_select::select"));

        check_fd(e);
    }

    return (0);
}