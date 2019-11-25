#ifndef CLIENT_SSL_H
#define CLIENT_SSL_H

/* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

#include <client/irc.h>

/* define HOME to be dir for key and cert files... */
// #define HOME "./.cert"

/* Make these what you want for cert & key files */
// #define CERTF HOME "/server.crt"
// #define KEYF HOME "/server.key"
// #define CACERT HOME "/ca.crt"

SSL_CTX *load_ssl();
int ssl_connect(t_env *e, t_fd *fd, int cs);

#endif
