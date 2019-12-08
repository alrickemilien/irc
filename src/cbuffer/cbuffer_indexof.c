#include <cbuffer/cbuffer.h>

static size_t cbuffer_indexof_head_below_tail(t_cbuffer *cbuf, const char *str)
{
    size_t i;
    size_t count;

    // When head is below tail
    i = 0;
    while (cbuf->tail > cbuf->head && i != cbuf->head)
    {
        count = 0;
        while (i + count != cbuf->head && str[count] != 0 &&
               str[count] == cbuf->buffer[i + count])
            count++;

        if (str[count] == 0)
            return (i);
        i++;
    }

    return ((size_t)-1);
}

size_t cbuffer_indexof(t_cbuffer *cbuf, const char *str)
{
    size_t i;
    size_t j;
    size_t count;

    i = cbuf->tail;
    while (i < CBUFFSIZE && i != cbuf->head)
    {
        count = 0;
        while (i + count < CBUFFSIZE && i + count != cbuf->head &&
               str[count] != 0 && str[count] == cbuf->buffer[i + count])
            count++;

        // When it is still ok on matching, need to go at start of buffer
        j = 0;
        while (str[count] != 0 && i + count == CBUFFSIZE &&
               str[count - 1] == cbuf->buffer[i + count - 1] &&
               j != cbuf->head && str[count] != 0 &&
               str[count] == cbuf->buffer[j])
        {
            count++;
            j++;
        }

        if (str[count] == 0)
            return (i);

        i++;
    }

    return (cbuffer_indexof_head_below_tail(cbuf, str));
}