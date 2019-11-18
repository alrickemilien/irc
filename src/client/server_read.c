#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void server_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    char   command[512];

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

    // logdebug("server_read:: %ld\n", cs);
    // logdebug("server_read:: databuffer tail BEFORE RECV is %ld\n", e->fds[cs].buf_read.tail);
    // logdebug("server_read:: databuffer head BEFORE RECV is %ld\n", e->fds[cs].buf_read.head);

    // logdebug("server_read:: cbuffer_size(&e->fds[cs].buf_read): %ld\n",
    //        cbuffer_size(&e->fds[cs].buf_read));

    if (r <= 0)
    {
        close(cs);
        clear_fd(&e->fds[cs]);
        logerror("Connection between client hand server has been lost\n");
        e->sock = -1;
        FD_CLR(cs, &e->fd_read);
        FD_CLR(cs, &e->fd_write);
        return;
    }

    index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A");

    while (index != (size_t)-1)
    {
        // printf("index: %ld\n", index);

        memset(command, 0, CBUFFSIZE);

        // Copy circular buffer command into local buffer
        if (e->fds[cs].buf_read.tail < index)
        {
            memcpy(command,
                   e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
                   index - e->fds[cs].buf_read.tail);
        }
        else
        {
            memcpy(command,
                   e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
                   CBUFFSIZE - e->fds[cs].buf_read.tail);
            memcpy(command + CBUFFSIZE - e->fds[cs].buf_read.tail,
                   e->fds[cs].buf_read.buffer, index);
        }

        logdebug("%s\n", command);

        // Drop command
        // +2 because of "\x0D\x0A" skipping
        cbuffer_dropn(&e->fds[cs].buf_read,
                      (e->fds[cs].buf_read.tail < index
                           ? index - e->fds[cs].buf_read.tail
                           : index + CBUFFSIZE - e->fds[cs].buf_read.tail) +
                          2);

        index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A");
    }
}
