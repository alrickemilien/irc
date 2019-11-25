#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cbuffer_ssl.h>

int server_write(t_env *e, size_t cs)
{
    size_t index;

    // logdebug("server_write::welcome, there is data to send at #%ld\n", cs);
    // logdebug("server_write::buf_write has %ld bytes\n",
    //          cbuffer_size(&e->fds[cs].buf_write));

    index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");
    // logdebug("server_write::index:: %ld\n", index);

    // cbuffer_debug(&e->fds[cs].buf_write);

    // if (index != (size_t)-1)
    // logdebug("client_write::index %ld\n", index);

    // The buffer is full without any end of command, flush it
    if (index == (size_t)-1)
    {
        if (e->fds[cs].buf_write.full)
        {
            cbuffer_reset(&e->fds[cs].buf_write);
            return (logerror(
                "[!] Buffer is reset because it is full without command\n"));
        }
        return (0);
    }

    // Reading each output of the buffer
    while (index != (size_t)-1)
    {
        // logdebug("server_write::index:: %ld\n", index);
        if (e->options.ssl)
            cbuffer_write_ssl(
                e->fds[cs].ssl, &e->fds[cs].buf_write,
                (e->fds[cs].buf_write.tail < index
                     ? index - e->fds[cs].buf_write.tail
                     : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                    2);
        else
            cbuffer_send(cs, &e->fds[cs].buf_write,
                         (e->fds[cs].buf_write.tail < index
                              ? index - e->fds[cs].buf_write.tail
                              : index + CBUFFSIZE - e->fds[cs].buf_write.tail) +
                             2,
                         0);

        index = cbuffer_indexof(&e->fds[cs].buf_write, "\x0D\x0A");
    }

    return (0);
}
