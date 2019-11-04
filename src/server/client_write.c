#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "irc.h"

void client_write(t_env *e, size_t cs)
{
    (void)e;
    (void)cs;

    size_t i;

    i = 0;
    while (i < e->maxfd)
    {
        // Send data to all clients
        if ((e->fds[i].type == FD_CLIENT) && (i != cs))
        {
            memcpy(e->fds[cs].buf_write, "I LOVE YOU!", 12);
            send(i, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0);
        }
        i++;
    }
}
