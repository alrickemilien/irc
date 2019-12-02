
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cbuffer/cbuffer_ssl.h>
#include <server/irc.h>

/*
** Data available on read on the socket cs
*/

int client_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    t_fd * fd;

    fd = &e->fds[cs];

    // printf("client_read::%ld\n", cs);
    // printf("databuffer tail BEFORE RECV is %ld\n", fd->buf_read.tail);
    // printf("databuffer head BEFORE RECV is %ld\n", fd->buf_read.head);

    // logdebug("client_read::cbuffer_debug\n");
    // cbuffer_debug(&fd->buf_read);

    index = -1;
    if (cbuffer_size(&fd->buf_read) != CBUFFSIZE &&
        (cbuffer_isempty(&fd->buf_read) ||
         (index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A")) == (size_t)-1))
    {
        // Receiving data from the client cs
        if (e->ssl_ctx)
            r = cbuffer_read_ssl(&fd->buf_read, fd->ssl);
        else
            r = cbuffer_recv(&fd->buf_read, cs);

        // logdebug("client_read::r:: %ld\n", r);
        // printf("client_read::%ld bytes has been received for %ld\n", r, cs);

        if (r <= 0)
        {
            disconnect(e, cs);
            return (0);
        }
    }

    // printf("data buffer tail is %ld\n", fd->buf_read.tail);
    // printf("data buffer head is %ld\n", fd->buf_read.head);
    // printf("data buffer is: %s\n", fd->buf_read.buffer);

    if (index == (size_t)-1)
        index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");

    if (index == (size_t)-1)
    {
        // The buffer is full without any end of command, flush it
        if (fd->buf_read.full)
        {
            logerror("[!] Buffer is reset because it is full without command");
            cbuffer_reset(&fd->buf_read);
        }
        return (0);
    }

    // Reading each command oof the buffer
    while (index != (size_t)-1)
    {
        if (irc_command(e, cs, index) == IRC_QUIT)
        {
            disconnect(e, cs);
            return (0);
        }

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
