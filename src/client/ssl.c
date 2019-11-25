#include <netdb.h>
#include <netinet/in.h>

#include <client/ssl.h>

SSL_CTX *ssl_init()
{
    const SSL_METHOD *method;
    SSL_CTX *         ctx;

    SSL_load_error_strings();

    SSLeay_add_ssl_algorithms();
    // method = TLSv1_2_client_method();
    method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);

    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    // SSL_CTX_load_verify_locations(ctx, CACERT, NULL);

    // if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0)
    // {
    //     ERR_print_errors_fp(stderr);
    //     exit(-2);
    // }

    // if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
    // {
    //     ERR_print_errors_fp(stderr);
    //     exit(-3);
    // }

    // if (!SSL_CTX_check_private_key(ctx))
    // {
    //     printf("Private key does not match the certificate public keyn");
    //     exit(-4);
    // }

    return ctx;
}

static int client_ssl_error(void)
{
    ERR_print_errors_fp(stderr);
    return (-1);
}

int ssl_connect(t_env *e, t_fd *fd, int cs)
{
    char *diag;
    X509 *server_cert;

    if ((e->ssl_ctx = ssl_init()) == (void *)0)
        return (client_ssl_error());

    if ((fd->ssl = SSL_new(e->ssl_ctx)) == (void *)0)
        return (client_ssl_error());

    SSL_set_fd(fd->ssl, cs);

    if (SSL_connect(fd->ssl) < 0)
        return (client_ssl_error());

    /* Get the cipher - opt */
    logdebug("SSL connection using %s\n", SSL_get_cipher(fd->ssl));

    /* Get server's certificate (note: beware of dynamic allocation) - opt */
    server_cert = SSL_get_peer_certificate(fd->ssl);

    if (server_cert == NULL)
        return (client_ssl_error());

    logdebug("Server certificate:\n");

    if ((diag = X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0)) ==
        (void *)0)
        return (client_ssl_error());
    logdebug("subject: %s\n", diag);
    OPENSSL_free(diag);

    if ((diag = X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0)) ==
        (void *)0)
        return (client_ssl_error());
    logdebug("issuer: %s\n", diag);
    OPENSSL_free(diag);

    /* We could do all sorts of certificate verification stuff here before
       deallocating the certificate. */

    X509_free(server_cert);

    return (0);
}
