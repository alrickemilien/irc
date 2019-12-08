/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cbuffer_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:04:22 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:04:24 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cbuffer/cbuffer.h>
#include <stdio.h>

void	cbuffer_debug(
		const t_cbuffer *cbuf)
{
	size_t	i;

	i = 0;
	printf("cbuf->tail: %ld\n"
			"cbuf->head: %ld\n"
			"cbuf->size: %ld\n"
			"cbuf->data:\n",
			cbuf->tail, cbuf->head,
			cbuffer_size(cbuf));
	i = 0;
	while (i < CBUFFSIZE)
	{
		if (cbuf->buffer[i] == 0)
			printf("%c", '.');
		else if (cbuf->buffer[i] == '\x0D')
			printf("%s", "\\x0D");
		else if (cbuf->buffer[i] == '\x0A')
			printf("%s", "\\x0A");
		else
			printf("%c", cbuf->buffer[i]);
		i++;
	}
	printf("\n");
}
