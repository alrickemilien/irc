/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipv4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:44:46 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 17:44:46 by aemilien         ###   ########.fr       */
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

int	client_ipv4(t_env *e)
{
	int					cs;
	struct sockaddr_in	sin;
	struct protoent		*pe;
	struct hostent		*hostnm;

	if ((hostnm = gethostbyname(e->options.host)) == NULL)
		return (logerrno("ipv4::gethostbyname"));
	if ((pe = (struct protoent *)getprotobyname("tcp")) == (void *)0)
		return (logerrno("ipv4::getprotobyname"));
	if ((cs = socket(AF_INET, SOCK_STREAM, pe->p_proto)) < 0)
		return (logerrno("ipv4::socket\n"));
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->options.port);
	sin.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
	if (connect(cs, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		return (logerrno("ipv4::connect"));
	if (e->options.ssl && ssl_connect(e, &e->fds[cs], cs) < 0)
		return (logerror("ipv4::ssl_connect\n"));
	e->sock = cs;
	e->fds[cs].type = FD_CLIENT;
	e->fds[cs].read = server_read;
	e->fds[cs].write = server_write;
	return (0);
}
