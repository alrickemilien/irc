
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server/irc.h"

void client_read(t_env *e, size_t cs)
{
    size_t r;
    size_t len;
    size_t i;
    char * ptr;

    // Receiving data from the client cs
    r = recv(cs, e->fds[cs].buf_read, BUF_SIZE - 1, 0);

    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        printf(e->is_tty ? "\x1b[31m"
                           "Client #%ld gone away"
                           "\x1B[0m\n"
                         : "Client #%ld gone away\n",
               cs);
    }

    ptr = strchr(e->fds[cs].buf_read, "\x0D\x0A");
    len = strlen(e->fds[cs].buf_read);
    if (!ptr)
    {
        // Drop the message, it is too long
        if (len == BUF_SIZE)
            memset(e->fds[cs].buf_read, 0, BUF_SIZE);
        return;
    }

    len = strlen(e->fds[cs].buf_read);
    if (e->fds[cs].buf_read[len - 2] && e->fds[cs].buf_read[len - 1])

        else
        {
            irc_command(e, cs, e->fds[cs].buf_read);

            i = 0;
            while (i < e->maxfd)
            {
                // Send data to all clients
                if ((e->fds[i].type == FD_CLIENT) && (i != cs))
                {
                    if (e->fds[cs].buf_read[r - 1] != 0x0A)
                        e->fds[cs].buf_read[r] = 0x0A;
                    else
                        e->fds[cs].buf_read[r] = 0;
                    e->fds[cs].buf_read[r + 1] = 0;

                    // printf("SEND\n");
                    send(i, e->fds[cs].buf_read, r + 2, 0);
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
