#ifndef CBUFFER_SSL_H
#define CBUFFER_SSL_H

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

#include <cbuffer/cbuffer.h>

int cbuffer_read_ssl(t_cbuffer *cbuf, SSL *ssl);
int cbuffer_write_ssl(SSL *ssl, t_cbuffer *cbuf, size_t n);

#endif
