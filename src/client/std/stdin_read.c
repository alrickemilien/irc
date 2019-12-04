#include <client/irc.h>

int stdin_read(t_env *e, size_t cs)
{
    size_t r;
    size_t index;
    char   command[512];

    // Receiving data from the client cs
    r = cbuffer_read(&e->fds[cs].buf_read, cs);

    if (r <= 0)
        return (r);

    index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A");

    while (index != (size_t)-1)
    {
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

        c2s(e, command);

        // Drop command
        // +1 because of "\x0A" skipping
        cbuffer_dropn(&e->fds[cs].buf_read,
                      (e->fds[cs].buf_read.tail < index
                           ? index - e->fds[cs].buf_read.tail
                           : index + CBUFFSIZE - e->fds[cs].buf_read.tail) +
                          1);
        index = cbuffer_indexof(&e->fds[cs].buf_read, "\x0A");
    }

    return (0);
}
