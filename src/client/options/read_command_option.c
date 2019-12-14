/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command_option.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 12:32:45 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 12:32:55 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <client/irc.h>

int	read_command_option(t_options *options, const char *value)
{
	if (value[0] == 0)
	{
		return (fprintf(stderr,
			"for the --command option: command must be a valid command: '%s'.\n",
			value));
	}
	strcat(options->command, value);
	strcat(options->command, "\x0A");
	return (0);
}
