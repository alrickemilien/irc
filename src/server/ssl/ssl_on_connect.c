#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <server/ssl.h>

void on_connect_ssl(SSL_CTX *ctx, t_fd *fd, int cs)
{
    int   err;
    X509 *client_cert;
    char *str;

    /* ----------------------------------------------- */
    /* TCP connection is ready. Do server side SSL. */

    fd->ssl = SSL_new(ctx);
    CHK_NULL(fd->ssl);

    SSL_set_fd(fd->ssl, cs);

    err = SSL_accept(fd->ssl);
    CHK_SSL(err);

    /* Get the cipher - opt */
    logdebug("SSL connection using %s\n", SSL_get_cipher(fd->ssl));

    /* Get client's certificate (note: beware of dynamic allocation) - opt */
    client_cert = SSL_get_peer_certificate(fd->ssl);
    if (client_cert != NULL)
    {
        logdebug("Client certificate:\n");

        str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
        CHK_NULL(str);
        logdebug("subject: %s\n", str);
        OPENSSL_free(str);

        str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
        CHK_NULL(str);
        logdebug("issuer: %s\n", str);
        OPENSSL_free(str);

        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */

        X509_free(client_cert);
    }
    else
        logdebug("Client does not have certificate.\n");
}