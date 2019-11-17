#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <server/irc.h>

void client_write(t_env *e, size_t cs)
{
    size_t index;

    // Send data to all clients
    if (e->fds[cs].type != FD_CLIENT)
        return;

    index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");

    if (index != (size_t)-1)
        printf("client_write::index %ld\n", index);

    // The buffer is full without any end of command, flush it
    if (index == (size_t)-1)
    {
        if (e->fds[cs].buf_write.full)
            cbuffer_reset(&e->fds[cs].buf_write);
        return;
    }

    cbuffer_send(cs, &e->fds[cs].buf_write,
                 (e->fds[cs].buf_write.tail < index
                      ? index - e->fds[cs].buf_write.tail
                      : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                     2,
                 0);

    // Drop command
    // +2 because of "\x0D\x0A" skipping
    cbuffer_dropn(&e->fds[cs].buf_write,
                  (e->fds[cs].buf_write.tail < index
                       ? index - e->fds[cs].buf_write.tail
                       : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                      2);
}
