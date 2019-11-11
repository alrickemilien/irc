#include <server/irc.h>

void do_select(t_env *e)
{
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;

    (void)timeout;

    // return the number of file descriptors
    // contained in the three returned descriptor sets
    e->r = XSAFE(
        -1, select(e->max + 1, &e->fd_read, &e->fd_write, (void *)0, (void*)0),
        "do_select::select");
}