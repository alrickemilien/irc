/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_backlog_option.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:58:31 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:58:33 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/irc.h>

int				read_backlog_option(
		t_options *options, const char *value)
{
	if (value[0] == 0)
		return (-1);
	if (ato32(value, (uint32_t*)&options->backlog) != 0
			|| options->backlog < 1
			|| options->port > 99999)
		return (fprintf(stderr, "--backlog: witing 1-99999'%s'.\n", value));
	return (0);
}
