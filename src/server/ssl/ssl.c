#include <netdb.h>
#include <netinet/in.h>

#include <server/ssl.h>

#define CACERT HOME "/ca.crt"

static SSL_CTX *create_SSL_context()
{
    const SSL_METHOD *method;

    SSL_load_error_strings();

    SSLeay_add_ssl_algorithms();
    // method = TLSv1_2_client_method();
    method = SSLv23_server_method();
    return (SSL_CTX_new(method));
}

void load_ssl(t_env *e)
{
    SSL_CTX *ctx;

    ctx = create_SSL_context();

    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        exit(2);
    }

    // SSL_CTX_set_ecdh_auto(ctx, 1);

    // SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    // SSL_CTX_load_verify_locations(ctx, CACERT, NULL);

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