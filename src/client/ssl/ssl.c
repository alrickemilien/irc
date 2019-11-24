#include <netdb.h>
#include <netinet/in.h>

#include <client/ssl.h>

void load_ssl(t_env *e)
{
    SSL_CTX *ctx;

    const SSL_METHOD *method;

    SSL_load_error_strings();

    SSLeay_add_ssl_algorithms();
    // method = TLSv1_2_client_method();
    method = SSLv23_client_method();

    ctx = SSL_CTX_new(method);

    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    // SSL_CTX_load_verify_locations(ctx, CACERT, NULL);

    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(-2);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(-3);
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        printf("Private key does not match the certificate public keyn");
        exit(-4);
    }

    e->ssl_ctx = ctx;
}