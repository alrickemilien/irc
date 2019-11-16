
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
    size_t index;

    index = -1;
    if (cbuffer_isempty(&e->fds[cs].buf_read) ||
        (index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A")) == (size_t)-1)
    {
        // Receiving data from the client cs
        r = cbuffer_recv(&e->fds[cs].buf_read, cs);

        printf("%ld bytes has been received for %ld\n", r, cs);
        printf("data buffer tail is %ld\n", e->fds[cs].buf_read.tail);
        printf("data buffer head is %ld\n", e->fds[cs].buf_read.head);
        printf("data buffer is: %s\n", e->fds[cs].buf_read.buffer);

        if (r <= 0)
            return (disconnect(e, cs));
    }

    if (index == (size_t)-1)
        index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A");

    if (index == (size_t)-1)
    {
        // The buffer is full without any end of command, flush it
        if (e->fds[cs].buf_read.full)
            cbuffer_reset(&e->fds[cs].buf_read);
        return;
    }

    if (irc_command(e, cs, index) == IRC_QUIT)
    {
        disconnect(e, cs);
        cbuffer_reset(&e->fds[cs].buf_read);
        return;
    }

    // Drop command
    // +2 because of "\x0D\x0A" skipping
    cbuffer_dropn(&e->fds[cs].buf_read,
                  (e->fds[cs].buf_read.tail < index
                      ? index - e->fds[cs].buf_read.tail
                      : index + CBUFFSIZE - e->fds[cs].buf_read.tail) + 2);
}
