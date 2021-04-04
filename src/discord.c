#include <assert.h>
#include <stdlib.h>
#include "discord.h"
#include "ssl.h"
#include "common.h"
#include "websocket.h"

discord_state_t *dstate = NULL;

void
discord_write (const char *msg)
{
  if (dstate == NULL)
    {
      debug_print ("can't write on uninitialized dstate");
      return;
    }
  ssl_tcp_write (dstate->ssl, msg);
}

void
discord_read (char *buf, size_t bufsize)
{
  if (dstate == NULL)
    {
      debug_print ("can't read data on uninitialized state");
      return;
    }
  ssl_tcp_read (dstate->ssl, buf, bufsize);
}

discord_state_t*
discord_init_state (const char *token)
{
  dstate = (discord_state_t *)malloc (sizeof (discord_state_t));
  if (dstate == NULL)
    {
      fputs ("failed to allocate discord_state_t", stderr);
      return NULL;
    }
  debug_print ("allocated discord_state_t");
  memset (dstate, 0, sizeof (discord_state_t));
  dstate->conn_state = DISCONNECTED;
  dstate->token = token;

  dstate->write_message = (write_function_t)discord_write;
  dstate->read_message = (read_function_t)discord_read;
  return dstate;
}

bool
discord_connect_gateway (discord_state_t *dstate)
{
  ws_upgrade_connection (dstate->sockfd, DISCORD_GATEWAY_HOSTNAME,
      "?v=" DISCORD_GATEWAY_VERSION
      "&encoding=" DISCORD_GATEWAY_ENC,
      dstate->write_message, dstate->read_message
      );
}

bool
discord_dtor_state (discord_state_t *state)
{
  if (state->conn_state == DISCONNECTED)
    {
      debug_print ("trying to free discord_state_t");
      free (state);
      return true;
    }
  else if (state->conn_state == CONNECTED)
    {
      ssl_close_tcp_connection (state->ssl);
      free (state);
      return true;
    }
  else if (state->conn_state == AUTHENTICATED)
    {
      assert (0);  /* unimplemented */
    }
}
