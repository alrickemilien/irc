/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemilien <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 15:05:48 by aemilien          #+#    #+#             */
/*   Updated: 2019/12/14 15:05:49 by aemilien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_SSL_H
# define CLIENT_SSL_H

# include <openssl/crypto.h>
# include <openssl/err.h>
# include <openssl/pem.h>
# include <openssl/rsa.h>
# include <openssl/ssl.h>
# include <openssl/x509.h>

# include <client/irc.h>

SSL_CTX	*load_ssl();
int		ssl_connect(t_env *e, t_fd *fd, int cs);

#endif
