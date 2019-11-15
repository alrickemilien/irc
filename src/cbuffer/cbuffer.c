#include <cbuffer.h>

// typedef struct s_cbuffer {
//     uint8_t buffer[CBUFFSIZE];
//     size_t head;
//     size_t tail;
//     bool full;
// } t_cbuffer;

void cbuffer_reset(t_cbuffer *cbuf)
{
    assert(cbuf);

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
    memset(buffer->data, 0, CBUFFSIZE);
}

size_t cbuffer_size(t_cbuffer *cbuf)
{
    size_t size;

    assert(cbuf);

    size = CBUFFSIZE;

    if (!cbuf->full)
    {
        if (cbuf->head >= cbuf->tail)
            size = (cbuf->head - cbuf->tail);
        else
            size = (CBUFFSIZE + cbuf->head - cbuf->tail);
    }

    return size;
}

void cbuffer_put(t_cbuffer *cbuf, uint8_t *data, size_t n)
{
    int count;

    assert(cbuf && cbuf->buffer);

    count = CBUFFSIZE - 1 - cbuf->head;
    memcpy(cbuf->buffer + cbuf->head, data, count > n ? n : count);

    if (count < n)
        memcpy(cbuf->buffer, data, n - count);
    cbuf->head = (cbuf->head + n) % CBUFFSIZE;

    if (cbuf->head < cbuf->tail)
        cbuf->tail = cbuf->head + 1;

    return (0);
}

/*
** on put, the head pointer is moved forward
**
** . . . . . . . . . . . . .
** |     |
** t     h
**
** . . . . . . . . . . . . .
** |               |
** t               h
**
** . . . . . . . . . . . . .
** |               |
** t               h
*/

int cbuffer_put_safe(t_cbuffer *cbuf, uint8_t *data, size_t n)
{
    int count;

    assert(cbuf && cbuf->buffer);

    count = 0;
    if (cbuf->head > cbuf->tail)
        count += CBUFFSIZE - 1 - cbuf->head;

    if (count + cbuf->tail < n)
        return (-1);

    if (count != 0)
    {
        memcpy(cbuf->buffer + cbuf->head, data, count > n ? n : count);
        cbuf->head += (count > n ? n : count);
    }

    if (count < n)
    {
        memcpy(cbuf->buffer, data,
               cbuf->tail > (n - count) ? (n - count) : cbuf->tail);
        cbuf->head = cbuf->tail > (n - count) ? (n - count) : cbuf->tail;
    }

    return (0);
}

bool cbuffer_isempty(t_cbuffer *cbuf)
{
    assert(cbuf);

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

// Receiving data from the client cs
int cbuffer_recv(t_cbuffer *cbuf, int cs)
{
    int r;

    // Not enough space for data fetch
    // Buffer still full
    if (cbuffer_size(buffer) == CBUFFSIZE)
        return (-1);

    if (cbuf->head > cbuf->tail)
        r = recv(cs, cbuf->buffer + cbuf->head, CBUFFSIZE - cbuf->head, 0);
    else
        r = recv(cs, cbuf->buffer + cbuf->head, cbuf->tail - cbuf->head, 0);

    if (r > 0)
        cbuf->head = (cbuf->head + r) % CBUFFSIZE;
    return (r);
}

// Receiving data from the client cs
int cbuffer_send(t_cbuffer *cbuf, int cs, size_t n)
{
    int r;

    // Not enough space for data fetch
    // Buffer still full
    if (cbuffer_size(buffer) == CBUFFSIZE)
        return (-1);

    send(cbuf->buffer + cbuf->tail);

    if (cbuf->head > cbuf->tail)
        r = send(cs, cbuf->buffer + cbuf->head, CBUFFSIZE - cbuf->head, 0);
    else
        r = send(cs, cbuf->buffer + cbuf->head, cbuf->tail - cbuf->head, 0);

    if (r > 0)
        cbuf->head = (cbuf->head + r) % CBUFFSIZE;
    return (r);
}
