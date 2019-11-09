
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
    char * ptr;

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

    printf("%ld available data for client %ld\n", r, cs);

    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        printf(e->is_tty ? "\x1b[31m"
                           "Client #%ld gone away"
                           "\x1B[0m\n"
                         : "Client #%ld gone away\n",
               cs);

        FD_CLR(cs, &e->fd_read);
        FD_CLR(cs, &e->fd_write);

        return;
    }

    ptr = strstr(e->fds[cs].buf_read.data, "\x0D\x0A");
    if (!ptr)
    {
        // Drop the message, it is too long
        if (e->fds[cs].buf_read.size == BUF_SIZE)
        {
            printf("Flushing buffer ...\n");
            cbuffer_flush(&e->fds[cs].buf_read);
        }
        return;
    }

    irc_command(e, cs, e->fds[cs].buf_read.data);
    cbuffer_nflush(&e->fds[cs].buf_read,
                   (size_t)(ptr - e->fds[cs].buf_read.data));
}
