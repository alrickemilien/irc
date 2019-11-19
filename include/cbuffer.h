#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

// Debug
#include <assert.h>
#include <stdio.h>

#define CBUFFSIZE 512

typedef struct s_cbuffer {
    uint8_t buffer[CBUFFSIZE];
    size_t head;
    size_t tail;
    bool full;
} t_cbuffer;

/// Reset the circular buffer to empty, head == tail. Data not cleared
/// Requires: cbuf is valid and created by circular_buf_init
void cbuffer_reset(t_cbuffer* cbuf);

/// Continues to add data if the buffer is full
/// Old data is overwritten
/// Requires: cbuf is valid and created by circular_buf_init
void cbuffer_put(t_cbuffer *cbuf, uint8_t *data, size_t n);

void cbuffer_putstr(t_cbuffer *cbuf, const char *str);

/// Rejects new data if the buffer is full
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns 0 on success, -1 if buffer is full
int circular_buf_put_safe(t_cbuffer* cbuf, uint8_t data);

/// CHecks if the buffer is empty
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns true if the buffer is empty
bool cbuffer_isempty(t_cbuffer* cbuf);

/// Check the number of elements stored in the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the current number of elements in the buffer
size_t cbuffer_size(const t_cbuffer* cbuf);

///
int cbuffer_send(int cs, t_cbuffer *cbuf, size_t n, int flags);

///
int cbuffer_recv(t_cbuffer *cbuf, int cs);
int cbuffer_read(t_cbuffer *cbuf, int cs);
size_t cbuffer_indexof(t_cbuffer *cbuf, const char *str);
void cbuffer_dropn(t_cbuffer *cbuf, size_t n);

void cbuffer_debug(const t_cbuffer *cbuf);

#endif  // CBUFFER_H_
