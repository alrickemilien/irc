#ifndef SERVER_SSL_H
#define SERVER_SSL_H

/* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

#include <client/irc.h>

/* define HOME to be dir for key and cert files... */
#define HOME "./.cert"

/* Make these what you want for cert & key files */
#define CERTF HOME "/server.crt"
#define KEYF HOME "/server.key"
#define CACERT HOME "/ca.crt"

#define CHK_NULL(x)  \
    if ((x) == NULL) \
    exit(1)
#define CHK_ERR(err, s) \
    if ((err) == -1)    \
    {                   \
        perror(s);      \
        exit(1);        \
    }
#define CHK_SSL(err)                 \
    if ((err) == -1)                 \
    {                                \
        ERR_print_errors_fp(stderr); \
        exit(2);                     \
    }

void load_ssl(t_env *e);
void connect_ssl(SSL_CTX *ctx, t_fd *fd, int cs);

#endif