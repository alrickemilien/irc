#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <server/irc.h>

int cbuffer_push(t_cbuffer *buffer, char *data, size_t size)
{
    if (size > BUF_SIZE - buffer->cursor)
        return (-1);
    memcpy(buffer->data + buffer->cursor, data, size);
    buffer->cursor += size;
    return (0);
}

int cbuffer_flush(t_cbuffer *buffer)
{
    memset(buffer->data, 0, BUF_SIZE);
    buffer->cursor = 0;
    return (0);
}

// Receiving data from the client cs
int cbuffer_recv(t_cbuffer *buffer, int cs)
{
    return recv(cs, buffer->data + buffer->cursor, BUF_SIZE - buffer->cursor,
                0);
}

int cbuffer_pflush(t_cbuffer *buffer, char *data, size_t size)
{
    if (cbuffer_push(buffer, data, size) < 0)
        return (-1);
    cbuffer_flush(buffer);
    return (0);
}