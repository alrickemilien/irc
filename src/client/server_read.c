#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <client/irc.h>
#include <cbuffer/cbuffer_ssl.h>

int server_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    char   command[512];
    t_fd * fd;

    fd = &e->fds[cs];

    // cbuffer_debug(&fd->buf_read);

    // Receiving data from the client cs
    if (e->options.ssl)
        r = cbuffer_read_ssl(&fd->buf_read, fd->ssl);
    else
        r = cbuffer_recv(&fd->buf_read, cs);

    if (r <= 0)
    {
        close(cs);
        clear_fd(fd);
        logerror("Connection between client and server has been lost");
        e->sock = -1;
        FD_CLR(cs, &e->fd_read);
        FD_CLR(cs, &e->fd_write);
        return (r);
    }

    // cbuffer_debug(&fd->buf_read);

    index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");

    while (index != (size_t)-1)
    {
        memset(command, 0, CBUFFSIZE);

        // Copy circular buffer command into local buffer
        if (fd->buf_read.tail < index)
        {
            memcpy(command, fd->buf_read.buffer + fd->buf_read.tail,
                   index - fd->buf_read.tail);
        }
        else
        {
            memcpy(command, fd->buf_read.buffer + fd->buf_read.tail,
                   CBUFFSIZE - fd->buf_read.tail);
            memcpy(command + CBUFFSIZE - fd->buf_read.tail, fd->buf_read.buffer,
                   index);
        }

        s2c(e, cs, command);

        // Drop command
        // +2 because of "\x0D\x0A" skipping
        cbuffer_dropn(&fd->buf_read,
                      (fd->buf_read.tail < index
                           ? index - fd->buf_read.tail
                           : index + CBUFFSIZE - fd->buf_read.tail) +
                          2);

        index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
    }
    return (0);
}
