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
    char err_buf[120];

    ERR_error_string(120, err_buf);

    return (logerror("client_ssl_error:: %s\n", err_buf));
}

static int logssl(char *str)
{
    if (str == (void *)0)
        return (-1);
    logdebug("%s\n", str);
    OPENSSL_free(str);
    return (0);
}

int ssl_connect(t_env *e, t_fd *fd, int cs)
{
    X509 *server_cert;

    loginfo("ssl_connect::connecting ...\n");

    if ((e->ssl_ctx = ssl_init()) == (void *)0)
        return (client_ssl_error());

    if ((fd->ssl = SSL_new(e->ssl_ctx)) == (void *)0)
        return (client_ssl_error());

    SSL_set_fd(fd->ssl, cs);

    if (SSL_connect(fd->ssl) < 0)
    {
        logdebug("efhzruileghergre::D\n");
        client_ssl_error();

        return (-1);
    }

    logdebug("ssl_connect::D\n");

    /* Get the cipher - opt */
    logdebug("SSL connection using %s\n", SSL_get_cipher(fd->ssl));

    /* Get server's certificate (note: beware of dynamic allocation) - opt */
    server_cert = SSL_get_peer_certificate(fd->ssl);

    if (server_cert == NULL)
        return (client_ssl_error());

    logdebug("Server certificate:\n");

    if (logssl(X509_NAME_oneline(X509_get_subject_name(server_cert), 0, 0)) < 0)
        return (client_ssl_error());

    if (logssl(X509_NAME_oneline(X509_get_issuer_name(server_cert), 0, 0)) < 0)
        return (client_ssl_error());

    /* We could do all sorts of certificate verification stuff here before
       deallocating the certificate. */

    X509_free(server_cert);

    return (0);
}
