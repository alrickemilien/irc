#include <cbuffer_ssl.h>
#include <server/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int client_write(t_env *e, size_t cs)
{
    size_t index;

    // Send data to all clients
    if (e->fds[cs].type != FD_CLIENT)
        return (0);

    index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");

    // cbuffer_debug(&e->fds[cs].buf_write);

    // if (index != (size_t)-1)
    // printf("client_write::index %ld\n", index);

    // The buffer is full without any end of command, flush it
    if ((index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A")) ==
        (size_t)-1)
    {
        if (e->fds[cs].buf_write.full)
        {
            logerror(
                "[!] Buffer is reset because it is full without command\n");
            cbuffer_reset(&e->fds[cs].buf_write);
        }
        return (0);
    }

    // Reading each output of the buffer
    while (index != (size_t)-1)
    {
        if (!e->ssl_ctx)
            cbuffer_send(cs, &e->fds[cs].buf_write,
                         (e->fds[cs].buf_write.tail < index
                              ? index - e->fds[cs].buf_write.tail
                              : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                             2,
                         0);
        else
            cbuffer_write_ssl(
                e->fds[cs].ssl, &e->fds[cs].buf_write,
                (e->fds[cs].buf_write.tail < index
                     ? index - e->fds[cs].buf_write.tail
                     : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                    2);

        index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");
    }

    // printf(":AFTER :\n");
    // cbuffer_debug(&e->fds[cs].buf_write);
    return (0);
}
