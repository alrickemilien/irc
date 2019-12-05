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

        if (r <= 0)
        {
            disconnect(e, cs);
            return (0);
        }
    }

    // logdebug("client_read::cbuffer_debug\n");
    // cbuffer_debug(&fd->buf_read);

    if (index == (size_t)-1)
        index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");

    // logdebug("index: %ld", index);

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

        cbuffer_drop_until(&fd->buf_read, "\x0D\x0A");

        index = cbuffer_indexof(&fd->buf_read, "\x0D\x0A");
    }

    return (0);
}
