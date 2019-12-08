/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:29:20 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/08 17:29:21 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SSL_H
# define SERVER_SSL_H

/*
** SSLeay stuff
*/

# include <openssl/crypto.h>
# include <openssl/err.h>
# include <openssl/pem.h>
# include <openssl/rsa.h>
# include <openssl/ssl.h>
# include <openssl/x509.h>

# include <server/irc.h>

/*
** define HOME to be dir for key and cert files...
*/

# define HOME "./.cert"

/*
** Make these what you want for cert & key files
*/

# define CERTF "./.cert/server.crt"
# define KEYF "./.cert/server.key"

int	ssl_init(t_env *e, const char *ssl_key_file, const char *ssl_crt_file);
int	ssl_on_connect(SSL_CTX *ctx, t_fd *fd, int cs);

#endif
