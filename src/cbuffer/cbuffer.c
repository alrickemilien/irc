#include <cbuffer.h>

void cbuffer_reset(t_cbuffer *cbuf)
{
    assert(cbuf);

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
    memset(cbuf->buffer, 0, CBUFFSIZE);
}

size_t cbuffer_size(const t_cbuffer *cbuf)
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
    size_t count;

    assert(cbuf);

    count = CBUFFSIZE - cbuf->head;
    memcpy(cbuf->buffer + cbuf->head, data, count > n ? n : count);

    // printf("count: %ld\n", count);
    // printf("cbuf->head: %ld\n", cbuf->head);
    // printf("cbuf->tail: %ld\n", cbuf->tail);

    // When all string has been copied
    if (count >= n)
    {
        cbuf->head += n;
        return;
    }

    if ((cbuf->head + n) % CBUFFSIZE >= cbuf->tail)
    {
        cbuf->head = cbuf->tail == 0 ? CBUFFSIZE : cbuf->tail - 1;
        count = cbuf->tail;
    }
    else
    {
        cbuf->head = (cbuf->head + n) % CBUFFSIZE;
        count = n - count;
    }
    if (count)
        memcpy(cbuf->buffer, data, n - count);
}

void cbuffer_putstr(t_cbuffer *cbuf, const char *str)
{
    size_t count;
    size_t n;

    assert(cbuf);

    n = strlen(str);
    count = CBUFFSIZE - cbuf->head;
    memcpy(cbuf->buffer + cbuf->head, str, count > n ? n : count);

    // printf("count: %ld\n", count);
    // printf("cbuf->head: %ld\n", cbuf->head);
    // printf("cbuf->tail: %ld\n", cbuf->tail);

    // When all string has been copied
    if (count >= n)
    {
        cbuf->head += n;
        return;
    }

    if ((cbuf->head + n) % CBUFFSIZE >= cbuf->tail)
    {
        cbuf->head = cbuf->tail == 0 ? CBUFFSIZE : cbuf->tail - 1;
        count = cbuf->tail;
    }
    else
    {
        cbuf->head = (cbuf->head + n) % CBUFFSIZE;
        count = n - count;
    }
    if (count)
        memcpy(cbuf->buffer, str, n - count);
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
    size_t count;

    assert(cbuf);

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

size_t cbuffer_indexof(t_cbuffer *cbuf, const char *str)
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

bool cbuffer_isempty(t_cbuffer *cbuf)
{
    assert(cbuf);

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

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

    cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;

    return (r);
}

// Drop n bytes of the buffer from tail
void cbuffer_dropn(t_cbuffer *cbuf, size_t n)
{
    // Not enough space for data fetch
    // Buffer still full
    if (cbuffer_isempty(cbuf))
        return;

    if (cbuf->tail + n > CBUFFSIZE)
        cbuf->head = (cbuf->tail + n) % CBUFFSIZE;
    cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;
}

// Drop n bytes of the buffer from tail
void cbuffer_debug(const t_cbuffer *cbuf)
{
    printf(
        "cbuf->tail: %ld\n"
        "cbuf->head: %ld\n"
        "cbuf->size: %ld\n",
        cbuf->tail, cbuf->head, cbuffer_size(cbuf));


    printf("cbuf->data:\n");

    for (size_t i = 0; i < CBUFFSIZE; i++)
    {
        if (cbuf->buffer[i] == 0)
            printf("%c", '.');
        else if (cbuf->buffer[i] == '\x0D')
            printf("%s", "\\x0D");
        else if (cbuf->buffer[i] == '\x0A')
            printf("%s", "\\x0A");
        else
            printf("%c", cbuf->buffer[i]);
    }
    printf("\n");
}
