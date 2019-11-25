#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <server/ssl.h>

#define CACERT HOME "/ca.crt"

static int ssl_server_error(void)
{
    ERR_print_errors_fp(stderr);
    return (-1);
}

int ssl_init(t_env *e, const char *ssl_key_file, const char *ssl_crt_file)
{
    SSL_CTX *         ctx;
    const SSL_METHOD *method;

    loginfo("load_ssl::ssl_key_file:: %s\n", ssl_key_file[0] ? ssl_key_file : KEYF);
    loginfo("load_ssl::ssl_crt_file:: %s\n", ssl_crt_file[0] ? ssl_crt_file : CERTF);

    SSL_load_error_strings();

    SSLeay_add_ssl_algorithms();

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);

    if (!ctx)
        return (ssl_server_error());

    // SSL_CTX_set_ecdh_auto(ctx, 1);

    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    // SSL_CTX_load_verify_locations(ctx, CACERT, NULL);

    if (SSL_CTX_use_certificate_file(ctx, ssl_crt_file[0] ? ssl_crt_file : CERTF,
                                     SSL_FILETYPE_PEM) <= 0)
        return (ssl_server_error());

    if (SSL_CTX_use_PrivateKey_file(ctx, ssl_key_file[0] ? ssl_key_file : KEYF,
                                    SSL_FILETYPE_PEM) <= 0)
        return (ssl_server_error());

    if (!SSL_CTX_check_private_key(ctx))
        return logerror(
            "Private key does not match the certificate public key\n");

    e->ssl_ctx = ctx;

    return (0);
}

int ssl_on_connect(SSL_CTX *ctx, t_fd *fd, int cs)
{
    int   err;
    X509 *client_cert;
    char *str;

    /* ----------------------------------------------- */
    /* TCP connection is ready. Do server side SSL. */
    if ((fd->ssl = SSL_new(ctx)) == (void *)0)
        return (ssl_server_error());

    SSL_set_fd(fd->ssl, cs);

    if ((err = SSL_accept(fd->ssl)) < 0)
        return (ssl_server_error());

    /* Get the cipher - opt */
    logdebug("SSL connection using %s\n", SSL_get_cipher(fd->ssl));

    /* Get client's certificate (note: beware of dynamic allocation) - opt */
    client_cert = SSL_get_peer_certificate(fd->ssl);
    if (client_cert != NULL)
    {
        logdebug("Client certificate:\n");

        if ((str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0,
                                     0)) == (void *)0)
        {
            X509_free(client_cert);
            return ssl_server_error();
        }

        logdebug("Subject: %s\n", str);
        OPENSSL_free(str);

        if ((str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0,
                                     0)) == (void *)0)
        {
            X509_free(client_cert);
            return ssl_server_error();
        }
        logdebug("Issuer: %s\n", str);
        OPENSSL_free(str);

        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */

        X509_free(client_cert);
    }
    else
        logdebug("Client does not have certificate.\n");

    return (0);
}

