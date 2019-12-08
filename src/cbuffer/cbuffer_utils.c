#include <cbuffer/cbuffer.h>

bool cbuffer_isempty(t_cbuffer *cbuf)
{
    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

void cbuffer_reset(t_cbuffer *cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
    memset(cbuf->buffer, 0, CBUFFSIZE);
}

size_t cbuffer_size(const t_cbuffer *cbuf)
{
    size_t size;

    size = CBUFFSIZE;

    if (!cbuf->full)
    {
        if (cbuf->head >= cbuf->tail)
            size = (cbuf->head - cbuf->tail);
        else
            size = (CBUFFSIZE + cbuf->head - cbuf->tail);
    }

    return (size);
}

// Drop n bytes of the buffer from tail
void cbuffer_drop_until(t_cbuffer *cbuf, const char *str)
{
    size_t index;
    size_t n;

    index = cbuffer_indexof(cbuf, str);
    n = cbuf->tail < index ? index - cbuf->tail
                           : index + CBUFFSIZE - cbuf->tail;
    return cbuffer_dropn(cbuf, n + strlen(str));
}
