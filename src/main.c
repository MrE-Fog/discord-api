#include <stdio.h>
#include <stdlib.h>
#include "ssl.h"
#include "tcp.h"
#include "discord.h"
#include "common.h"
#include "websocket.h"

int
create_ssl_socket (SSL * ssl, const char * hostname, unsigned short port)
{
  const char *converted_port = int_to_string (port);
  int sockfd = create_tcp_connection (hostname, converted_port);
  if (sockfd == -1)
    {
      fputs ("couldn't connect to Discord's gateway", stderr);
      return sockfd;
    }
  else if (!ssl_connect_tcp_connection (ssl, sockfd))
    {
      fputs ("couldn't wrap socket over OpenSSL", stderr);
      close (sockfd);
      return -1;
    }
  return sockfd;
}

int
main (int argc, char ** argv)
{
  if (argc < 2)
    {
      fputs ("./main <token>", stderr);
      return EXIT_FAILURE;
    }

  ssl_init_openssl ();

  discord_state_t *dstate = discord_init_state (argv[1]);
  dstate->ssl = ssl_create_ssl ();
  dstate->ctx = SSL_get_SSL_CTX (dstate->ssl);
  debug_print ("created dstate and constructed SSL context");

  dstate->sockfd = create_ssl_socket (dstate->ssl, DISCORD_GATEWAY_HOSTNAME, DISCORD_GATEWAY_PORT);

  if (!discord_connect_gateway (dstate))
    goto exit_proc;
  
  printf ("connected to gateway!");

exit_proc:
  debug_print ("freeing SSL context and destroying dstate");
  ssl_free_context (dstate->ssl);
  discord_dtor_state (dstate);
  return EXIT_SUCCESS;
}
