#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void server_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

    printf("server_read::%ld\n", cs);
    printf("databuffer tail BEFORE RECV is %ld\n", e->fds[cs].buf_read.tail);
    printf("databuffer head BEFORE RECV is %ld\n", e->fds[cs].buf_read.head);

    printf("cbuffer_size(&e->fds[cs].buf_read): %ld\n",
           cbuffer_size(&e->fds[cs].buf_read));

    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        logerror("Connection between client hand server has been lost\n");
        e->sock = -1;
        // FD_CLR(cs, &e->fd_read);
        // FD_CLR(cs, &e->fd_write);
    }

    index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A");

    printf("INDEX after: %ld\n", index);

    // Drop command
    // +2 because of "\x0D\x0A" skipping
    cbuffer_dropn(&e->fds[cs].buf_read,
                  (e->fds[cs].buf_read.tail < index
                       ? index - e->fds[cs].buf_read.tail
                       : index + CBUFFSIZE - e->fds[cs].buf_read.tail) +
                      2);
}