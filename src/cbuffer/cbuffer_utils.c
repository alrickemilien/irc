#include <cbuffer/cbuffer.h>

bool	cbuffer_isempty(
		t_cbuffer *cbuf)
{
	return (!cbuf->full && (cbuf->head == cbuf->tail));
}

void	cbuffer_reset(
		t_cbuffer *cbuf)
{
	cbuf->head = 0;
	cbuf->tail = 0;
	cbuf->full = false;
	memset(cbuf->buffer, 0, CBUFFSIZE);
}

size_t	cbuffer_size(
		const t_cbuffer *cbuf)
{
	size_t	size;

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

size_t	cbuffer_indexof(
		t_cbuffer *cbuf,
		const char *str)
{
    size_t i;
    size_t j;
    size_t count;

    if (str[0] == 0)
        return (-1);

    // printf("str: %s\n", str);

    i = cbuf->tail;
    while (i < CBUFFSIZE && i != cbuf->head)
    {
        count = 0;
        while (i + count < CBUFFSIZE && i + count != cbuf->head &&
               str[count] != 0 && str[count] == cbuf->buffer[i + count])
            count++;

        // printf("========================\n");
        // printf("i: %ld\n", i);
        // printf("cbuf->buffer[i]: %c\n", cbuf->buffer[i]);
        // printf("cbuf->buffer[i + count - 1]: %c\n",
        //        cbuf->buffer[i + count - 1]);
        // printf("str[count - 1]: %c\n", str[count - 1]);
        // printf("count: %ld\n", count);
        // printf("i + count: %ld\n", i + count);

        // When it is still ok on matching, need to go at start of buffer
        if (str[count] != 0 && i + count == CBUFFSIZE &&
            str[count - 1] == cbuf->buffer[i + count - 1])
        {
            j = 0;
            while (j != cbuf->head && str[count] != 0 &&
                   str[count] == cbuf->buffer[j])
            {
                count++;
                j++;
            }
        }

        if (str[count] == 0)
            return (i);

        i++;
    }

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
