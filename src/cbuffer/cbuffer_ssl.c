/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuffer_ssl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:03:21 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:03:21 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cbuffer/cbuffer.h>
#include <cbuffer/cbuffer_ssl.h>

int	cbuffer_read_ssl(
		t_cbuffer *cbuf,
		SSL *ssl)
{
	int	r;

	if (cbuf->head >= cbuf->tail)
	{
		r = SSL_read(ssl, cbuf->buffer + cbuf->head,
				CBUFFSIZE - cbuf->head);
	}
	else
	{
		r = SSL_read(ssl, cbuf->buffer + cbuf->head,
				cbuf->tail - cbuf->head);
	}
	if (r > 0)
		cbuf->head = (cbuf->head + r) % CBUFFSIZE;
	return (r);
}

int	cbuffer_write_ssl(
		SSL *ssl,
		t_cbuffer *cbuf,
		size_t n)
{
	int		r;
	size_t	count;
	char	to_send[CBUFFSIZE];

	if (cbuffer_isempty(cbuf))
		return (0);
	count = CBUFFSIZE - cbuf->tail < n
		? CBUFFSIZE - cbuf->tail : n;
	memset(to_send, 0, sizeof(to_send));
	memcpy(to_send, cbuf->buffer + cbuf->tail, count);
	if (count < n)
		memcpy(to_send + count, cbuf->buffer, n - count);
	r = SSL_write(ssl, to_send, n);
	if (r < 0)
		return (r);
	if (cbuf->tail + n >= CBUFFSIZE)
		cbuf->head = (cbuf->tail + n) % CBUFFSIZE;
	cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;
	return (r);
}
