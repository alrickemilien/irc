#include <netdb.h>
#include <netinet/in.h>

#include <client/ssl.h>

void connect_ssl(SSL_CTX *ctx, t_fd *fd, int cs)
{
    int   err;
    char *str;
    X509 *server_cert;

    fd->ssl = SSL_new(ctx);
    CHK_NULL(fd->ssl);

    printf("a\n");

    SSL_set_fd(fd->ssl, cs);

    err = SSL_connect(fd->ssl);
    CHK_SSL(err);

    printf("b\n");

    /* Get the cipher - opt */
    logdebug("SSL connection using %s\n", SSL_get_cipher(fd->ssl));

    /* Get server's certificate (note: beware of dynamic allocation) - opt */
    server_cert = SSL_get_peer_certificate(fd->ssl);
    CHK_NULL(server_cert);
    logdebug("Server certificate:\n");

    str = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0);
    CHK_NULL(str);
    logdebug("subject: %s\n", str);
    OPENSSL_free(str);

    str = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0);
    CHK_NULL(str);
    logdebug("issuer: %s\n", str);
    OPENSSL_free(str);

    /* We could do all sorts of certificate verification stuff here before
       deallocating the certificate. */

    X509_free(server_cert);
}