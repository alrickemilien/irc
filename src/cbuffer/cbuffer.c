#include <cbuffer.h>

// typedef struct s_cbuffer {
//     uint8_t buffer[CBUFFSIZE];
//     size_t head;
//     size_t tail;
//     bool full;
// } t_cbuffer;

#pragma mark - Private Functions -

static void advance_pointer(t_cbuffer *cbuf)
{
    assert(cbuf);

    if (cbuf->full)
    {
        cbuf->tail = (cbuf->tail + 1) % CBUFFSIZE;
    }

    cbuf->head = (cbuf->head + 1) % CBUFFSIZE;

    // We mark full because we will advance tail on the next time around
    cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(t_cbuffer *cbuf)
{
    assert(cbuf);

    cbuf->full = false;
    cbuf->tail = (cbuf->tail + 1) % CBUFFSIZE;
}

#pragma mark - APIs -

void cbuffer_reset(t_cbuffer *cbuf)
{
    assert(cbuf);

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
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
    assert(cbuf && cbuf->buffer);

    cbuf->buffer[cbuf->head] = data;

    advance_pointer(cbuf);
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
    int r;
    int count;

    assert(cbuf && cbuf->buffer);

    r = -1;

    if (circular_buf_full(cbuf))
        return (-1);

    if ( % CBUFFERSIZE) {

    }

	if(cbuf->full)
    {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }

	cbuf->head = (cbuf->head + 1) % cbuf->max;


    count = CBUFFSIZE - cbuf->head;

	if(cbuf->full)


    memcpy(cbuf->buffer + cbuf->head, data, size);

    cbuf->head

    buffer->size += size;

    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
    r = 0;

    return r;
}

int circular_buf_get(t_cbuffer *cbuf, uint8_t *data)
{
    assert(cbuf && data && cbuf->buffer);

    int r = -1;

    if (!circular_buf_empty(cbuf))
    {
        *data = cbuf->buffer[cbuf->tail];
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

bool cbuffer_isempty(t_cbuffer *cbuf)
{
    assert(cbuf);

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

int cbuffer_push(t_cbuffer *buffer, char *data, size_t size)
{
    if (size > BUF_SIZE - buffer->size)
        return (-1);
    memcpy(buffer->data + buffer->size, data, size);
    buffer->size += size;
    return (0);
}

int cbuffer_flush(t_cbuffer *buffer)
{
    memset(buffer->data, 0, BUF_SIZE);
    buffer->size = 0;
    return (0);
}

int cbuffer_nflush(t_cbuffer *buffer, size_t n)
{
    memmove(buffer->data, buffer->data + n, BUF_SIZE - n);
    memset(buffer->data + BUF_SIZE - n, 0, n);

    buffer->size = buffer->size < n ? 0 : buffer->size - n;
    return (0);
}

// Receiving data from the client cs
int cbuffer_recv(t_cbuffer *buffer, int cs)
{
    int r;

    r = recv(cs, buffer->data + buffer->size, BUF_SIZE - buffer->size, 0);

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
