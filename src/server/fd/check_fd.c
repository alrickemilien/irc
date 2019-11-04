#include "irc.h"

/*
** Loop over file descriptors
** 
*/

void check_fd(t_env *e)
{
    size_t i;

    i = 0;
    while ((i < e->maxfd) && (e->r > 0))
    {
        if (FD_ISSET(i, &e->fd_read))
            e->fds[i].read(e, i);
        if (FD_ISSET(i, &e->fd_write))
            e->fds[i].write(e, i);
        if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
            e->r--;
        i++;
    }
}
