/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipv4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:17:07 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 11:36:43 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <server/ssl.h>

static const int	g_reuseaddr = 1;
static const int	g_reuseport = 1;

int					server_ipv4(
		const t_options *options, t_env *e)
{
	int					sock;
	struct sockaddr_in	sin;
	struct protoent		*pe;

	if ((pe = (struct protoent *)getprotobyname("tcp")) == (void *)0)
		return (logerrno("server_ipv4::getprotobyname"));
	if ((sock = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
		return (logerrno("server_ipv4::socket"));
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &g_reuseaddr, sizeof(int));
	if (TARGET_OS_MAC)
		setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &g_reuseport, sizeof(int));
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
