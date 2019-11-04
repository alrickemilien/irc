#include <string.h>
#include <sys/select.h>
#include "irc.h"

void init_fd(t_env *e)
{
    size_t i;

    i = 0;
    e->max = 0;
    FD_ZERO(&e->fd_read);
    FD_ZERO(&e->fd_write);
    while (i < e->maxfd)
    {
        if (e->fds[i].type != FD_FREE)
        {
            FD_SET(i, &e->fd_read);

            // Add the write fd only if write buffer is available
            if (strlen(e->fds[i].buf_write) > 0)
            {
                FD_SET(i, &e->fd_write);
            }
            e->max = MAX(e->max, i);
        }
        i++;
    }
}
