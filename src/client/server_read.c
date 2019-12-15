#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cbuffer/cbuffer_ssl.h>
#include <client/irc.h>
#include <client/ui/panel.h>

void disconnect(t_env *e, int cs)
{
    if (e->options.gui && e->registered)
        ui_set_status(e->ui, 1);
    close(cs);
    clear_fd(&e->fds[cs]);
    logerror("Connection between client and server has been lost");
    e->sock = -1;
    FD_CLR(cs, &e->fd_read);
    FD_CLR(cs, &e->fd_write);
}

int server_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    char   command[512];
    t_fd * fd;

    fd = e->self;

    // cbuffer_debug(&fd->buf_read);

    // Receiving data from the client cs
    if (e->options.ssl)
        r = cbuffer_read_ssl(&fd->buf_read, fd->ssl);
    else
        r = cbuffer_recv(&fd->buf_read, cs);

    if (r <= 0)
    {
        disconnect(e, cs);
        return (r);
    }

    // logdebug("server_read:: cbuffer_debug ::");
    // cbuffer_debug(&fd->buf_read);

    index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");

    if (index == (size_t)-1)
    {
        // The buffer is full without any end of command, flush it
        if (cbuffer_size(&fd->buf_read) >= CBUFFSIZE)
        {
            logerror("[!] Buffer is reset because it is full without command");
            cbuffer_reset(&fd->buf_read);
        }
        return (0);
    }

    // logdebug("index: %ld", index);

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

        s2c(e, command);

        cbuffer_drop_until(&fd->buf_read, "\x0D\x0A");

        // logdebug("server_read:: after cbuffer_dropn ::");
        // cbuffer_debug(&fd->buf_read);

        index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
    }
    return (0);
}
