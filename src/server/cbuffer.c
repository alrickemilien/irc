#include <server/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int cbuffer_push(t_cbuffer *buffer, char *data, size_t size)
{
    if (size > BUF_SIZE - buffer->cursor)
        return (-1);
    memcpy(buffer->data + buffer->cursor, data, size);
    buffer->size += size;
    return (0);
}

int cbuffer_flush(t_cbuffer *buffer)
{
    memset(buffer->data, 0, BUF_SIZE);
    buffer->cursor = 0;
    buffer->size = 0;
    return (0);
}

int cbuffer_nflush(t_cbuffer *buffer, size_t n)
{
    memcpy(buffer->data, buffer->data + n, (BUF_SIZE - n) * sizeof(char));
    buffer->cursor = 0;
    buffer->size = buffer->size < n ? 0 : buffer->size - n;
    return (0);
}

// Receiving data from the client cs
int cbuffer_recv(t_cbuffer *buffer, int cs)
{
    int r;

    r = recv(cs, buffer->data + buffer->size, BUF_SIZE - buffer->cursor, 0);

    if (r > 0)
        buffer->size += r;
    return (r);
}

int cbuffer_pflush(t_cbuffer *buffer, char *data, size_t size)
{
    if (cbuffer_push(buffer, data, size) < 0)
        return (-1);
    cbuffer_flush(buffer);
    return (0);
}