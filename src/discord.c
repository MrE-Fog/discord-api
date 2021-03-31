#include <assert.h>
#include <stdlib.h>
#include "discord.h"
#include "ssl.h"
#include "common.h"

discord_state_t*
discord_init_state (const char *token)
{
  discord_state_t *state = (discord_state_t *)malloc (sizeof (discord_state_t));
  if (state == NULL)
    {
      fputs ("failed to allocate discord_state_t", stderr);
      return NULL;
    }
  debug_print ("allocated discord_state_t");
  memset (state, 0, sizeof (discord_state_t));
  state->conn_state = DISCONNECTED;
  state->token = token;
  return state;
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
