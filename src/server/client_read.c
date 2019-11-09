
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server/irc.h"

/*
** Data available on read on the socket cs
*/

void client_read(t_env *e, size_t cs)
{
    size_t r;
    size_t i;
    char * ptr;

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

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

    ptr = strchr(e->fds[cs].buf_read.data, "\x0D\x0A");
    if (!ptr)
    {
        // Drop the message, it is too long
        if (e->fds[cs].buf_read.cursor + 1 == BUF_SIZE)
        {
            printf("Dropping command ...\n");
            cbuffer_flush(&e->fds[cs].buf_read);
        }
        return;
    }

    irc_command(e, cs, e->fds[cs].buf_read);
    cbuffer_flush(&e->fds[cs].buf_read);

    // i = 0;
    // while (i < e->maxfd)
    // {
    //     // Send data to all clients
    //     if ((e->fds[i].type == FD_CLIENT) && (i != cs))
    //     {

    //         if (e->fds[cs].buf_read[r - 1] != 0x0A)
    //             e->fds[cs].buf_read[r] = 0x0A;
    //         else
    //             e->fds[cs].buf_read[r] = 0;
    //         e->fds[cs].buf_read[r + 1] = 0;

    //         // printf("SEND\n");
    //         // send(i, e->fds[cs].buf_read, r + 2, 0);
    //     }
    //     i++;
    // }
}
