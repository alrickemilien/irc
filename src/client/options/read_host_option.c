/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_host_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:50:41 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 17:50:42 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <client/irc.h>

int	read_host_option(t_options *options, const char *value)
{
	struct addrinfo	hints;
	struct addrinfo	*res;

	if (value[0] == 0)
		return (fprintf(stderr, "--host: need ip: '%s'.\n", value));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(value, NULL, &hints, &res) != 0)
		return (fprintf(stderr, "--host: must be a ip adress: '%s'.\n", value));
	while (res)
	{
		if (res->ai_family == AF_INET6)
			options->ipv6 = true;
		if (res->ai_family != AF_INET)
			return (fprintf(stderr, "--host: must be ip: '%s'.\n", value));
		res = res->ai_next;
	}
	strcpy(options->host, value);
	return (0);
}
