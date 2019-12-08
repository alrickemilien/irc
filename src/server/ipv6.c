/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipv6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:19:13 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:19:14 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <server/ssl.h>

int	server_ipv6(const t_options *options, t_env *e)
{
	int					sock;
	int					reuseaddr;
	struct sockaddr_in6	sin;
	struct protoent		*pe;
#ifdef __APPLE__
	int					reuseport;
#endif  // __APPLE__

	if ((pe = (struct protoent *)getprotobyname("tcp")) == (void *)0)
		return (logerrno("server_ipv6::getprotobyname"));
	if ((sock = socket(AF_INET6, SOCK_STREAM, pe->p_proto)) == -1)
		return (logerrno("server_ipv6::socket"));
	reuseaddr = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
#ifdef __APPLE__
	reuseport = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(reuseport));
#endif  // __APPLE__
	memset(&sin, 0, sizeof(sin));
	sin.sin6_family = AF_INET6;
	sin.sin6_addr = in6addr_any;
	sin.sin6_port = htons(options->port);
	if (bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in6)) == -1)
		return (logerrno("server_ipv6::bind"));
	if (listen(sock, options->backlog) == -1)
		return (logerrno("server_ipv6::listen"));
	if (options->ssl &&
			ssl_init(e, options->ssl_key_file, options->ssl_crt_file) < 0)
		return (logerrno("server_ipv6::ssl_init"));
	e->fds[sock].type = FD_SERV;
	e->fds[sock].read = on_connect;
	return (0);
}
