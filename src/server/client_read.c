
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "irc.h"

void client_read(t_env *e, size_t cs)
{
    int r;
    size_t i;

    r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        printf("client #%ld gone away\n", cs);
    }
    else
    {
        i = 0;
        while (i < e->maxfd)
        {
            // Send data to all clients
            if ((e->fds[i].type == FD_CLIENT) && (i != cs))
                send(i, e->fds[cs].buf_read, r, 0);
            i++;
        }
    }
}
