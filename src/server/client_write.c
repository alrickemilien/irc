#include <cbuffer/cbuffer_ssl.h>
#include <server/irc.h>

int client_write(t_env *e, size_t cs)
{
    size_t index;
    t_fd * fd;

    fd = &e->fds[cs];

    // Send data to all clients
    if (fd->type != FD_CLIENT)
        return (0);

    index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");

    logdebug("client_write::cbuffer_debug");
    cbuffer_debug(&fd->buf_write);

    if (index == (size_t)-1)
        exit(1);

    // The buffer is full without any end of command, flush it
    if ((index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A")) == (size_t)-1)
    {
        if (fd->buf_write.full)
        {
            logerror("[!] Buffer is reset because it is full without command");
            cbuffer_reset(&fd->buf_write);
        }
        return (0);
    }

    // Reading each output of the buffer
    while (index != (size_t)-1)
    {
        if (!e->ssl_ctx &&
            cbuffer_send_until_str(cs, &fd->buf_write, "\x0D\x0A",
                                   MSG_NOSIGNAL) <= 0)
            return (-1);
        if (e->ssl_ctx &&
            cbuffer_write_ssl(fd->ssl, &fd->buf_write,
                              (fd->buf_write.tail <= index
                                   ? index - fd->buf_write.tail
                                   : index + CBUFFSIZE - fd->buf_write.tail) +
                                  2) <= 0)
            return (-1);

        logdebug("After cbuffer_send::");
        cbuffer_debug(&fd->buf_write);

        index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");
    }

    // printf(":AFTER :\n");
    // cbuffer_debug(&fd->buf_write);
    return (0);
}
