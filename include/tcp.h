#ifndef __TCP_H
#define __TCP_H

#include <sys/socket.h>
#include <netdb.h>

int
create_tcp_connection (const char *, const char *);

#endif  /* __TCP_H */
