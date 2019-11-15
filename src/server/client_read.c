
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

    // printf("%ld bytes has been received for %ld\n", r, cs);
    // printf("data buffer size is %ld\n", e->fds[cs].buf_read.size);
    // printf("data buffer is: %s\n", e->fds[cs].buf_read.data);

    if (r <= 0)
        return disconnect(e, cs);

    if ((ptr = strstr(e->fds[cs].buf_read.data, "\x0D\x0A")) == (void *)0)
    {
        // Drop the message, it is too long
        if (e->fds[cs].buf_read.size == BUF_SIZE)
        {
            printf("Flushing buffer ...\n");
            cbuffer_flush(&e->fds[cs].buf_read);
        }
        return;
    }

    // Reading each command oof the buffer
    while (ptr)
    {
        if (irc_command(e, cs, e->fds[cs].buf_read.data) == IRC_QUIT)
        {
            disconnect(e, cs);

            cbuffer_nflush(&e->fds[cs].buf_read,
                           (size_t)(ptr - e->fds[cs].buf_read.data) + 2);
            return;
        }

        cbuffer_nflush(&e->fds[cs].buf_read,
                       (size_t)(ptr - e->fds[cs].buf_read.data) + 2);
        ptr = strstr(e->fds[cs].buf_read.data, "\x0D\x0A");
    }
}
