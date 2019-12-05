#include <cbuffer/cbuffer.h>

// Receiving data from the client cs
int cbuffer_recv(t_cbuffer *cbuf, int cs)
{
    int r;

    if (cbuf->head >= cbuf->tail)
        r = recv(cs, cbuf->buffer + cbuf->head, CBUFFSIZE - cbuf->head, 0);
    else
        r = recv(cs, cbuf->buffer + cbuf->head, cbuf->tail - cbuf->head, 0);

    if (r > 0)
        cbuf->head = (cbuf->head + r) % CBUFFSIZE;
    return (r);
}

// Receiving data from the client cs
int cbuffer_read(t_cbuffer *cbuf, int cs)
{
    int r;

    // logdebug("cbuffer_read::cbuffer head BEFO: %ld\n", cbuf->head);
    // logdebug("cbuffer_read::cbuffer tail BEFO: %ld\n", cbuf->tail);

    if (cbuf->head >= cbuf->tail)
        r = read(cs, cbuf->buffer + cbuf->head, CBUFFSIZE - cbuf->head);
    else
        r = read(cs, cbuf->buffer + cbuf->head, cbuf->tail - cbuf->head);

    if (r > 0)
        cbuf->head = (cbuf->head + r) % CBUFFSIZE;

    // logdebug("cbuffer_read::cbuffer head AFTER: %ld\n", cbuf->head);
    // logdebug("cbuffer_read::cbuffer tail AFTER: %ld\n", cbuf->tail);

    return (r);
}

// Receiving data from the client cs
int cbuffer_send(int cs, t_cbuffer *cbuf, size_t n, int flags)
{
    int    r;
    size_t count;
    char   to_send[CBUFFSIZE];

    // Buffer empty
    if (cbuffer_isempty(cbuf))
        return (0);

    count = CBUFFSIZE - cbuf->tail < n ? CBUFFSIZE - cbuf->tail : n;
    // printf("Sending %ld bytes to #%d\n", count, cs);

    memset(to_send, 0, sizeof(to_send));
    memcpy(to_send, cbuf->buffer + cbuf->tail, count);

    if (count < n)
        memcpy(to_send + count, cbuf->buffer, n - count);

    // printf("to_send: %s\n", to_send);

    r = send(cs, to_send, n, flags);

    if (r < 0)
        return (r);

    // When send over than cbuf->head
    if (cbuf->tail + n >= CBUFFSIZE &&
        (cbuf->tail + n) % CBUFFSIZE > cbuf->head)
        cbuf->head = (cbuf->tail + n) % CBUFFSIZE;
    cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;

    return (r);
}

int cbuffer_send_until_str(int cs, t_cbuffer *cbuf, const char *str, int flags)
{
    size_t index;
    size_t n;

    index = cbuffer_indexof(cbuf, str);
    n = cbuf->tail <= index ? index - cbuf->tail
                            : index + CBUFFSIZE - cbuf->tail;
    return cbuffer_send(cs, cbuf, n + strlen(str), flags);
}
