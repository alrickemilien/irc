#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdarg.h>

#define CBUFFSIZE 512

typedef struct  s_cbuffer {
    uint8_t     buffer[CBUFFSIZE];
    size_t      head;
    size_t      tail;
    bool        full;
}               t_cbuffer;

/*
** Reset the circular buffer to empty, head == tail. Data not cleared
** Requires: cbuf is valid and created by circular_buf_init
*/

void cbuffer_reset(t_cbuffer* cbuf);

/*
** Continues to add data if the buffer is full
** Old data is overwritten
** Requires: cbuf is valid and created by circular_buf_init
*/

void cbuffer_put(t_cbuffer *cbuf, const uint8_t *data, size_t n);

int cbuffer_putstr(t_cbuffer *cbuf, const char *str);

/*
** Rejects new data if the buffer is full
** Requires: cbuf is valid and created by circular_buf_init
** Returns 0 on success, -1 if buffer is full
*/

int circular_buf_put_safe(t_cbuffer* cbuf, const uint8_t data);

/*
** Checks if the buffer is empty
** Requires: cbuf is valid and created by circular_buf_init
** Returns true if the buffer is empty
*/

bool cbuffer_isempty(t_cbuffer* cbuf);

/*
** Check the number of elements stored in the buffer
** Requires: cbuf is valid and created by circular_buf_init
** Returns the current n
*/
size_t cbuffer_size(const t_cbuffer* cbuf);

/*
** Set n bytes using send() command
*/

int cbuffer_send(int cs, t_cbuffer *cbuf, size_t n, int flags);
int cbuffer_send_until_str(int cs, t_cbuffer *cbuf, const char *str, int flags);

/*
** Fill buffer as much as possible using with recv 
*/

int cbuffer_recv(t_cbuffer *cbuf, int cs);
int cbuffer_read(t_cbuffer *cbuf, int cs);

size_t cbuffer_indexof(t_cbuffer *cbuf, const char *str);

void cbuffer_dropn(t_cbuffer *cbuf, size_t n);
void cbuffer_drop_until(t_cbuffer *cbuf, const char *str);

void cbuffer_debug(const t_cbuffer *cbuf);

int cbuffer_putcmd(t_cbuffer *cbuf, const char *fmt, ...);

int cbuffer_putvcmd(t_cbuffer *cbuf, const char *fmt, va_list ap);

#endif
