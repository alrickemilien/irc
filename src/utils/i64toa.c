/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i64toa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 13:19:22 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 13:20:35 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <irc.h>

static void	reverse_string(char *str, size_t n)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = n - 1;
	while (start < end)
	{
		str[start] = str[start] ^ str[end];
		str[end] = str[end] ^ str[start];
		str[start] = str[start] ^ str[end];
		start++;
		end--;
	}
}

int			i64toa(
	uint64_t nbr, char *buffer, size_t buffer_size, uint64_t base)
{
	size_t	i;
	int		rem;

	memset(buffer, 0, buffer_size);
	i = 0;
	if (nbr == 0)
	{
		buffer[i++] = '0';
		return (i);
	}
	while (nbr != 0 && i < buffer_size)
	{
		rem = nbr % base;
		buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		nbr = nbr / base;
	}
	if (i == buffer_size && nbr != 0)
		return (-1);
	reverse_string(buffer, i);
	return (i);
}
