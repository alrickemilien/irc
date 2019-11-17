#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void server_write(t_env *e, size_t cs)
{
    size_t index;
    // printf("There is data to sen to the server\n");
    // printf("buf_write has %ld bytes\n", strlen(e->fds[cs].buf_write));
    // printf("buf_write: %s\n", e->fds[cs].buf_write);

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
        return;
    }

    // Reading each output of the buffer
    while (index != (size_t)-1)
    {
        cbuffer_send(cs, &e->fds[cs].buf_write,
                     (e->fds[cs].buf_write.tail < index
                          ? index - e->fds[cs].buf_write.tail
                          : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                         2,
                     0);

        index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");
    }
}