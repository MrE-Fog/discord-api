#include <stdio.h>
#include <stdbool.h>
#include "ssl.h"
#include "common.h"

void
ssl_free_context (SSL * ssl)
{
  debug_print ("freeing SSL object/context");
  SSL_CTX *ctx = SSL_get_SSL_CTX (ssl);
  int sfd = SSL_get_fd (ssl);
  if (sfd >= 0)
    {
      debug_print ("closing TCP socket");
      SSL_shutdown (ssl);	/* pray that this returns 0 */
      close (sfd);
    }
  if (ctx != NULL)
    SSL_CTX_free (ctx);
  if (ssl != NULL)
    SSL_free (ssl);
}

void
ssl_init_openssl (void)
{
  SSL_library_init ();
  SSL_load_error_strings ();
  OpenSSL_add_all_algorithms ();
  debug_print ("initialized OpenSSL");
}

SSL *
ssl_create_ssl (void)
{
  const SSL_METHOD *method = TLS_method ();
  SSL_CTX *context = SSL_CTX_new (method);

  if (context == NULL)
    {
      fputs ("SSL_CTX_new() failed", stderr);
      ERR_print_errors_fp (stderr);
      return NULL;
    }

  SSL *ssl = SSL_new (context);
  if (ssl == NULL)
    {
      fputs ("SSL_new() failed", stderr);
      ssl_free_context (ssl);
      return NULL;
    }
  return ssl;
}

bool
ssl_connect_tcp_connection (SSL * ssl, int sockfd)
{
  SSL_set_fd (ssl, sockfd);
  int status = SSL_connect (ssl);
  if (status != 1)
    {
      SSL_get_error (ssl, status);
      ERR_print_errors_fp (stderr);
      return false;
    }
  debug_print ("successfully wrapped socket over OpenSSL");
  return true;
}

void
ssl_close_tcp_connection (SSL * ssl)
{
  debug_print ("shutting down TCP connection");
  int sockfd = SSL_get_fd (ssl);
  SSL_shutdown (ssl);
  close (sockfd);
}

void
ssl_tcp_write (SSL * ssl, const char * data)
{
  SSL_write (ssl, data, strlen (data));
}

void
ssl_tcp_read (SSL * ssl, char * buf, size_t bufsize)
{
  SSL_read (ssl, buf, bufsize);
}
