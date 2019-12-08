/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuffer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:03:52 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:04:34 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cbuffer/cbuffer.h>
#include <stdio.h>

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

void	cbuffer_put(
	t_cbuffer *cbuf,
	const uint8_t *data,
	size_t n)
{
	size_t	count;

	count = CBUFFSIZE - cbuf->head;
	memcpy(cbuf->buffer + cbuf->head, data, count > n ? n : count);
	if (count >= n)
	{
		cbuf->head += n;
		return ;
	}
	if ((cbuf->head + n) % CBUFFSIZE >= cbuf->tail)
	{
		cbuf->head = cbuf->tail == 0 ? CBUFFSIZE : cbuf->tail - 1;
		memcpy(cbuf->buffer, data + count, cbuf->tail);
	}
	else
	{
		cbuf->head = (cbuf->head + n) % CBUFFSIZE;
		memcpy(cbuf->buffer, data + count, n - count);
	}
}

int		cbuffer_putstr(t_cbuffer *cbuf, const char *str)
{
	int	count;
	int	n;

	n = strlen(str);
	count = CBUFFSIZE - cbuf->head;
	memcpy(cbuf->buffer + cbuf->head, str, count > n ? n : count);
	if (count >= n)
	{
		cbuf->head += n;
		return (n);
	}
	if ((cbuf->head + n) % CBUFFSIZE >= cbuf->tail)
	{
		cbuf->head = cbuf->tail == 0 ? CBUFFSIZE : cbuf->tail - 1;
		count = cbuf->tail;
		memcpy(cbuf->buffer, str + count, cbuf->tail);
	}
	else
	{
		cbuf->head = (cbuf->head + n) % CBUFFSIZE;
		memcpy(cbuf->buffer, str + count, n - count);
	}
	return (n);
}

int		cbuffer_putcmd(
		t_cbuffer *cbuf,
		const char *fmt,
		...)
{
	va_list	ap;
	int		len;
	char	msg[512];

	memset(msg, 0, 512);
	va_start(ap, fmt);
	len = vsnprintf(msg, 512, fmt, ap);
	va_end(ap);
	if (len < 0)
		return (-1);
	cbuffer_put(cbuf, (uint8_t *)msg, len);
	return (len);
}

int		cbuffer_putvcmd(
		t_cbuffer *cbuf,
		const char *fmt,
		va_list ap)
{
	int		len;
	char	msg[512];

	memset(msg, 0, 512);
	len = vsnprintf(msg, 512, fmt, ap);
	if (len < 0)
		return (-1);
	cbuffer_put(cbuf, (uint8_t *)msg, len);
	return (len);
}

void	cbuffer_dropn(
		t_cbuffer *cbuf,
		size_t n)
{
	if (cbuffer_isempty(cbuf))
		return ;
	if (cbuf->tail + n > CBUFFSIZE && (cbuf->tail + n) % CBUFFSIZE > cbuf->head)
		cbuf->head = (cbuf->tail + n) % CBUFFSIZE;
	cbuf->tail = (cbuf->tail + n) % CBUFFSIZE;
}
