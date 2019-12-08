/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 14:15:36 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 14:38:04 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <server/ssl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

static int	ssl_server_error(void)
{
	ERR_print_errors_fp(stderr);
	return (-1);
}

int			ssl_init(
		t_env *e,
		const char *ssl_key_file,
		const char *ssl_crt_file)
{
	SSL_CTX				*ctx;
	const SSL_METHOD	*method;

	logdebug("load_ssl::ssl_key_file:: %s",
			ssl_key_file[0] ? ssl_key_file : KEYF);
	logdebug("load_ssl::ssl_crt_file:: %s",
			ssl_crt_file[0] ? ssl_crt_file : CERTF);
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	method = SSLv23_server_method();
	ctx = SSL_CTX_new(method);
	if (!ctx)
		return (ssl_server_error());
	if (SSL_CTX_use_certificate_file(
		ctx, ssl_crt_file[0] ? ssl_crt_file : CERTF, SSL_FILETYPE_PEM) <= 0)
		return (ssl_server_error());
	if (SSL_CTX_use_PrivateKey_file(ctx, ssl_key_file[0] ? ssl_key_file : KEYF,
				SSL_FILETYPE_PEM) <= 0)
		return (ssl_server_error());
	if (!SSL_CTX_check_private_key(ctx))
		return (logerror("Private key not match crt public key"));
	e->ssl_ctx = ctx;
	return (0);
}

int			ssl_on_connect(
		SSL_CTX *ctx,
		t_fd *fd,
		int cs)
{
	int		err;
	X509	*crt;

	if ((fd->ssl = SSL_new(ctx)) == (void *)0)
		return (ssl_server_error());
	SSL_set_fd(fd->ssl, cs);
	if ((err = SSL_accept(fd->ssl)) < 0)
		return (ssl_server_error());
	if ((crt = SSL_get_peer_certificate(fd->ssl)) == NULL)
		return (0);
	X509_free(crt);
	return (0);
}
