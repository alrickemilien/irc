/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_bind_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:56:58 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:56:59 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <server/irc.h>

int	read_bind_option(
		t_options *options,
		const char *value)
{
	struct addrinfo	hints;
	struct addrinfo	*res;

	if (value[0] == 0)
		return (fprintf(stderr, "--bind: need ip addr: '%s'.\n", value));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(value, NULL, &hints, &res) != 0)
		return (fprintf(stderr, "--bind: need valid ip '%s'.\n", value));
	while (res)
	{
		if (res->ai_family == AF_INET6)
			options->ipv6 = true;
		else if (res->ai_family != AF_INET)
			return (fprintf(stderr, "--bind: need valid ip: '%s'.\n", value));
		res = res->ai_next;
	}
	strcpy(options->bind, value);
	return (0);
}
