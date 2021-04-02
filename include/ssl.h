#ifndef __SSL_H
#define __SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <unistd.h>

void ssl_free_context (SSL *);

void ssl_init_openssl (void);

SSL *ssl_create_ssl (void);

bool ssl_connect_tcp_connection (SSL * ssl, int sockfd);

void ssl_tcp_write (SSL * ssl, const char *data);
void ssl_tcp_read (SSL * ssl, char *buf, size_t bufsize);

void ssl_close_tcp_connection (SSL * ssl);

#endif  /* __SSL_H */
