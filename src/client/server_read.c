#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void server_read(t_env *e, size_t cs)
{
    size_t r;

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

    printf("%ld bytes has been received for %ld\n", r, cs);
    printf("data buffer size is %ld\n", e->fds[cs].buf_read.size);
    printf("data buffer is: %s\n", e->fds[cs].buf_read.data);

    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        printf(e->is_tty
                   ? "\x1b[31m"
                     "Connection between client hand server has been lost"
                     "\x1B[0m\n"
                   : "Connection between client hand server has been lost\n");
        e->sock = -1;
        // FD_CLR(cs, &e->fd_read);
        // FD_CLR(cs, &e->fd_write);
    }

    cbuffer_flush(&e->fds[cs].buf_read);
}