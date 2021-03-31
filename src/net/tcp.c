#include <stdio.h>
#include <unistd.h>
#include "tcp.h"
#include "common.h"

int
create_tcp_connection (const char *hostname, const char *port)
{
  struct addrinfo *res = { 0 };
  const int status = getaddrinfo (hostname, port, NULL, &res);
  int sockfd;

  for (struct addrinfo * host = res; host != NULL; host = host->ai_next)
    {
      sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
      if (sockfd < 0)
	      {
      	  debug_print ("failed socket ()");
      	  continue;
      	}
      else if (!connect (sockfd, host->ai_addr, host->ai_addrlen))
      	{
      	  freeaddrinfo (res);
      	  return sockfd;
      	}
      debug_print ("failed connect (), closing sockfd");
      close (sockfd);
    }
  freeaddrinfo (res);
  return -1;
}
