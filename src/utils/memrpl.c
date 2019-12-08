/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memrpl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 13:19:15 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 13:19:16 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void	*memrpl(
	char *dest, size_t dest_size,
	const char *src, size_t src_size)
{
	memset(dest, 0, dest_size);
	return (memcpy(dest, src, src_size));
}
