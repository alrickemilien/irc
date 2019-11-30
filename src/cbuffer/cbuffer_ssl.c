#include <cbuffer/cbuffer.h>
#include <cbuffer/cbuffer_ssl.h>

// Receiving data from the client cs
int cbuffer_read_ssl(t_cbuffer *cbuf, SSL *ssl)
{
    int r;

    if (cbuf->head >= cbuf->tail)
        r = SSL_read(ssl, cbuf->buffer + cbuf->head, CBUFFSIZE - cbuf->head);
    else
        r = SSL_read(ssl, cbuf->buffer + cbuf->head, cbuf->tail - cbuf->head);

    if (r > 0)
        cbuf->head = (cbuf->head + r) % CBUFFSIZE;
    return (r);
}

// Receiving data from the client cs
int cbuffer_write_ssl(SSL *ssl, t_cbuffer *cbuf, size_t n)
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

    printf("cbuffer_write_ssl:: %s", to_send);

    r = SSL_write(ssl, to_send, n);

    printf("cbuffer_write_ssl:: send ok");


    if (r < 0)
        return (r);

    if (cbuf->tail + n >= CBUFFSIZE)
        cbuf->head = (cbuf->tail + n) % CBUFFSIZE;
    cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;

    return (r);
}
