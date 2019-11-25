/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_arch_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 14:52:58 by aemilien          #+#    #+#             */
/*   Updated: 2019/09/15 17:20:35 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int				read_port_option(
		t_options *options, const char *value)
{
	if (value[0] == 0)
		return (-1);
	if (ato32(value, (uint32_t*)&options->port) != 0 || options->port < 1000 || options->port > 99999)
		return (fprintf(stderr,
			"for the --port option: port must be a vakue between 1000 an 99999'%s'.\n", value));
	return (0);	
}
