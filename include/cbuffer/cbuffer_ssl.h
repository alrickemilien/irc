#ifndef CBUFFER_SSL_H_
#define CBUFFER_SSL_H_

#include <cbuffer/cbuffer.h>

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

int cbuffer_read_ssl(t_cbuffer *cbuf, SSL *ssl);
int cbuffer_write_ssl(SSL *ssl, t_cbuffer *cbuf, size_t n);

#endif  // CBUFFER_H_
