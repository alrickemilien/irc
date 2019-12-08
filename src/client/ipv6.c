/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipv6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:43:39 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 17:43:40 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <client/irc.h>
#include <client/ssl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int	client_ipv6(t_env *e)
{
	int					cs;
	struct sockaddr_in6	addr;
	struct protoent		*pe;

	loginfo("client_ipv6::");
	if ((pe = (struct protoent *)getprotobyname("tcp")) == (void *)0)
		return (logerrno("ipv6::getprotobyname"));
	if ((cs = socket(AF_INET6, SOCK_STREAM, pe->p_proto)) < 0)
		return (logerrno("ipv6::socket"));
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(e->options.port);
	inet_pton(AF_INET6, e->options.host, &addr.sin6_addr);
	if (connect(cs, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (logerrno("ipv6::connect"));
	if (e->options.ssl)
		ssl_connect(e, &e->fds[cs], cs);
	e->sock = cs;
	e->fds[cs].type = FD_CLIENT;
	e->fds[cs].read = server_read;
	e->fds[cs].write = server_write;
	return (0);
}
