#include <server/irc.h>

/*
** Loop over file descriptors
*/

void check_fd(t_env *e)
{
    size_t i;

    i = 0;
    while ((i < e->maxfd))
    {
        if (FD_ISSET(i, &e->fd_read))
        {
            e->fds[i].read(e, i);
        }

        if (FD_ISSET(i, &e->fd_write))
        {
            e->fds[i].write(e, i);
        }

        if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
        {
            e->fds[i].last_activity = (unsigned long)time(NULL);
            // e->r--;
        }

        if (e->fds[i].last_activity != 0 &&
            ((unsigned long)time(NULL) - e->fds[i].last_activity) >=
                MAX_TOLERATED_INACTIVITY_TIME_MS)
            disconnect(e, i);

        i++;
    }
}
