/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_arch_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 14:52:58 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 15:01:25 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int				read_port_option(
		t_options *options, const char *value)
{
	if (value[0] == 0)
		return (-1);
	if (ato32(value, (uint32_t*)&options->port) != 0
			|| options->port < 1000
			|| options->port > 99999)
		return (fprintf(stderr, "--port: need 1000-99999'%s'.\n", value));
	return (0);
}
