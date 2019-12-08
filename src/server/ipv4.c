/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipv4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:17:07 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:18:05 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <server/ssl.h>

int	server_ipv4(const t_options *options, t_env *e)
{
	int					sock;
	struct sockaddr_in	sin;
	struct protoent		*pe;
	int					reuseaddr;
#ifdef __APPLE__
	int					reuseport;
#endif  // __APPLE__

	if ((pe = (struct protoent *)getprotobyname("tcp")) == (void *)0)
		return (logerrno("server_ipv4::getprotobyname"));
	if ((sock = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
		return (logerrno("server_ipv4::socket"));
	reuseaddr = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
#ifdef __APPLE__
	reuseport = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(reuseport));
#endif  // __APPLE__
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(options->port);
	if (bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1)
		return (logerrno("server_ipv4::bind"));
	if (listen(sock, options->backlog) == -1)
		return (logerrno("server_ipv4::listen"));
	if (options->ssl &&
			ssl_init(e, options->ssl_key_file, options->ssl_crt_file) < 0)
		return (logerrno("server_ipv4::ssl_init"));
	e->fds[sock].type = FD_SERV;
	e->fds[sock].read = on_connect;
	return (0);
}
