#include <server/irc.h>

/*
** Loop over file descriptors
*/

void check_fd(t_env *e)
{
    size_t              i;
    const unsigned long t = (unsigned long)time(NULL);

    i = 0;
    while (i < e->maxfd)
    {
        if (FD_ISSET(i, &e->fd_read))
        {
            e->fds[i].read(e, i);
            e->fds[i].last_activity = t;
        }

        if (FD_ISSET(i, &e->fd_write))
        {
            e->fds[i].write(e, i);
        }

        if (e->fds[i].last_activity != 0 &&
            (t - e->fds[i].last_activity) >= TOLERATED_INACTIVITY_TIME_S &&
            (e->fds[i].last_ping_activity == 0 ||
             (t - e->fds[i].last_ping_activity) >= PING_INTERVAL_S))
        {
            ping(e, i);
            e->fds[i].last_ping_activity = t;
        }

        if (e->fds[i].last_activity != 0 &&
            (t - e->fds[i].last_activity) >= MAX_TOLERATED_INACTIVITY_TIME_S)
            disconnect(e, i);

        i++;
    }
}
