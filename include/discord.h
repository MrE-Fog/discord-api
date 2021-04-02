#ifndef __DISCORD_H
#define __DISCORD_H

#include <stdbool.h>
#include <openssl/ssl.h>
#include "websocket.h"

typedef enum {
  DISCONNECTED,
  CONNECTED,
  AUTHENTICATED
} connection_state_t;

typedef struct discord_state
{
  const char *token;
  SSL *ssl;
  SSL_CTX *ctx;
  int sockfd;
  connection_state_t conn_state;

  write_function_t write_message;
  read_function_t read_message;
  void (*parse_message)(struct discord_state *state, const char *message);
  void (*disconnect)(struct discord_state *state);
} discord_state_t;

discord_state_t *discord_init_state (const char *token);

bool discord_dtor_state (discord_state_t*);

bool discord_connect_gateway (discord_state_t*);

void discord_write (const char*);
void discord_read (char *, size_t);

#endif  /* __DISCORD_H */
