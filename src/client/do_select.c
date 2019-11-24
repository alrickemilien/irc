#include <client/irc.h>

void clear_fd(t_fd *fd)
{
    fd->registered = 0;
    fd->type = FD_FREE;
    fd->read = NULL;
    fd->write = NULL;
}

void init_fd(t_env *e)
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

            // Add the write fd only if write buffer is available
            if (cbuffer_size(&e->fds[i].buf_write) > 0)
                FD_SET(i, &e->fd_write);
            e->max = MAX(e->maxfd, i);
        }
        i++;
    }
}

void check_fd(t_env *e)
{
    size_t i;

    i = 0;
    while ((i < e->maxfd) && (e->r > 0))
    {
        if (FD_ISSET(i, &e->fd_read))
        {
            logdebug("check_fd::#%ld:: available data for read\n", i);
            e->fds[i].read(e, i);
        }
        if (FD_ISSET(i, &e->fd_write))
        {
            logdebug("check_fd::#%ld:: available data for write\n", i);

            e->fds[i].write(e, i);
        }
        if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
            e->r--;
        i++;
    }
}

void do_select(t_env *e)
{
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 500;

    (void)timeout;

    init_fd(e);

    // return the number of file descriptors
    // contained in the three returned descriptor sets
    e->r = XSAFE(
        -1, select(e->maxfd, &e->fd_read, &e->fd_write, (void *)0, &timeout),
        "do_select::select");

    check_fd(e);
}
