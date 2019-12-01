#ifndef CLIENT_SSL_H
#define CLIENT_SSL_H

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

#include <client/irc.h>

SSL_CTX *load_ssl();
int     ssl_connect(t_env *e, t_fd *fd, int cs);

#endif
