#include <string.h>
#include <stdlib.h>
#include <openssl/rand.h>  /* RAND_priv_bytes */
#include <openssl/bio.h>
#include <openssl/evp.h>  /* base64 encoding */
#include "common.h"
#include "base64.h"
#include "websocket.h"

char*
ws_generate_key (void)
{
  char rand_bytes[17] = {0};
  if (RAND_priv_bytes ((unsigned char*)rand_bytes, 16) == -1)
    {
      debug_print ("uhm the RAND_priv_bytes() isn't working :?");
      return NULL;
    }
  char *b64_rand_bytes = (char *)malloc (BASE64_ENCODE_LEN (sizeof (rand_bytes) - 1));
  base64_encode (b64_rand_bytes, rand_bytes, sizeof (rand_bytes) - 1);
  return b64_rand_bytes;
}

bool
ws_upgrade_connection (int sockfd, const char * host, const char * path,
    write_function_t write_fn, read_function_t read_fn)
{
  char local_path[64] = {0};
  char *upgrade_req = (char *)malloc (GENERIC_HEADER_SIZE);
  const char *websocket_key = ws_generate_key ();
  
  strncat (local_path, path, sizeof (local_path) - 1);
  sprintf (upgrade_req,
      "GET /%s " HEADER_VERSION         HEADER_ENDL
      "Host: %s"                        HEADER_ENDL
      "Upgrade: websocket"              HEADER_ENDL
      "Connection: Upgrade"             HEADER_ENDL
      "Sec-WebSocket-Key: %s"           HEADER_ENDL
      "Sec-WebSocket-Version: 13"       HEADER_ENDL HEADER_ENDL,
        local_path, host, websocket_key
      );

  debug_print ("sending Websocket upgrade request");
  write_fn (upgrade_req);
  free (upgrade_req);

  char *server_response = (char *)malloc (GENERIC_HEADER_SIZE);
  read_fn (server_response, GENERIC_HEADER_SIZE);

  struct websocket_response *verif = wsutil_parse_response (server_response)  /* tbd */

  free (server_response);
  free ((void *)websocket_key);
  return true;
} 
