#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void stdin_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    char   command[512];

    // Receiving data from the client cs
    r = cbuffer_recv(&e->fds[cs].buf_read, cs);

    printf("stdin_read::%ld\n", cs);
    printf("stdin_read buffer tail BEFORE RECV is %ld\n",
           e->fds[cs].buf_read.tail);
    printf("stdin_read buffer head BEFORE RECV is %ld\n",
           e->fds[cs].buf_read.head);

    printf("stdin_read cbuffer_size(&e->fds[cs].buf_read): %ld\n",
           cbuffer_size(&e->fds[cs].buf_read));

    if (r <= 0)
        return;

    index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A");

    if (index == -1)
        index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0D\x0A");

    printf("INDEX: %ld\n", index);

    memset(command, 0, CBUFFSIZE);

    // Copy circular buffer command into local buffer
    if (e->fds[cs].buf_read.tail < index)
        memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
               index - e->fds[cs].buf_read.tail);
    else
    {
        memcpy(command, e->fds[cs].buf_read.buffer + e->fds[cs].buf_read.tail,
               CBUFFSIZE - e->fds[cs].buf_read.tail);
        memcpy(command + CBUFFSIZE - e->fds[cs].buf_read.tail,
               e->fds[cs].buf_read.buffer, index);
    }

    printf("command: %s\n", command);

    // Drop command
    // +2 because of "\x0D\x0A" skipping
    cbuffer_dropn(&e->fds[cs].buf_read,
                  (e->fds[cs].buf_read.tail < index
                       ? index - e->fds[cs].buf_read.tail
                       : index + CBUFFSIZE - e->fds[cs].buf_read.tail) +
                      2);
}
