#include <cbuffer/cbuffer_ssl.h>
#include <client/irc.h>

int server_write(t_env *e, size_t cs)
{
    size_t index;
    t_fd * fd;

    fd = &e->fds[cs];

    index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");

    // cbuffer_debug(&e->fds[cs].buf_write);

    // The buffer is full without any end of command, flush it
    if (index == (size_t)-1)
    {
        if (fd->buf_write.full)
        {
            cbuffer_reset(&fd->buf_write);
            return (logerror(
                "[!] Buffer is reset because it is full without command"));
        }
        return (0);
    }

    // Reading each output of the buffer
    while (index != (size_t)-1)
    {
        if (!e->options.ssl &&
            cbuffer_send_until_str(cs, &fd->buf_write, "\x0D\x0A",
                                   MSG_NOSIGNAL) <= 0)
            return (-1);
        if (e->options.ssl &&
            cbuffer_write_ssl(fd->ssl, &fd->buf_write,
                              (fd->buf_write.tail < index
                                   ? index - fd->buf_write.tail
                                   : index + CBUFFSIZE - fd->buf_write.tail) +
                                  2) <= 0)
            return (-1);
        index = cbuffer_indexof(&fd->buf_write, "\x0D\x0A");
    }

    return (0);
}
