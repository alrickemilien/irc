
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "irc.h"

void client_read(t_env *e, size_t cs)
{
    int    r;
    size_t i;

    r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
    printf("Received: %s\n", e->fds[cs].buf_read);
    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        printf("Client #%ld gone away\n", cs);
    }
    else
    {
        i = 0;
        while (i < e->maxfd)
        {
            // Send data to all clients
            if ((e->fds[i].type == FD_CLIENT) && (i != cs))
            {
                printf("SEND\n");
                send(i, e->fds[cs].buf_read, r, 0);
            }
            i++;
        }
    }

    if (r > 0)
    {
        // rotate buffer
        memcpy(e->fds[cs].buf_read, e->fds[cs].buf_read + r, sizeof(char) * r);
        memset(e->fds[cs].buf_read + r, 0, sizeof(char) * BUF_SIZE - r);
    }
}
