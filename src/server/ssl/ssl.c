#include <netdb.h>
#include <netinet/in.h>

#include <server/ssl.h>

void load_ssl(t_env *e)
{
    SSL_CTX *   ctx;
    const SSL_METHOD *meth;

    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();
    meth = SSLv23_server_method();
    ctx = SSL_CTX_new(meth);

    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        exit(2);
    }
    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(3);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(4);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {
        fprintf(stderr,
                "Private key does not match the certificate public key\n");
        exit(5);
    }

    e->ssl_ctx = ctx;
}