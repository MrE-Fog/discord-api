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
  char rand_bytes[16] = {0};
  if (RAND_priv_bytes ((unsigned char*)rand_bytes, 16) == -1)
    {
      debug_print ("uhm the RAND_priv_bytes() isn't working :?");
      return NULL;
    }
  char *b64_rand_bytes = (char *)calloc (sizeof (char), BASE64_ENCODE_LEN (sizeof (rand_bytes)) + 1);
  base64_encode (b64_rand_bytes, rand_bytes, sizeof (rand_bytes));
  return b64_rand_bytes;
}

websocket_msg_t
ws_read_message (const char *msg)
{
  websocket_msg_t ws_msg = {
      .fin = msg[0] & 0x80,
      .rsv1 = msg[0] & 0x40,
      .rsv2 = msg[0] & 0x20,
      .rsv3 = msg[0] & 0x10,
      .opcode = msg[0] & 0x0f,

      .mask = msg[1] & 0x80,
      .payload_length = msg[1] & 0x7f,
    };

  if (ws_msg.payload_length <= 125)
    {
      if (ws_msg.mask)
        {
          ws_msg.masking_key = ((uint64_t)msg[2] << 24) & ((uint64_t)msg[3] << 16)
              & ((uint64_t)msg[4] << 8) & msg[5];
          ws_msg.payload_data = &(((char *)msg)[6]);  
        }
      else 
        {
          ws_msg.payload_data = &(((char*)msg)[2]);
        }
    }
  
  wsutil_print_message (ws_msg);
  /* ISSUE: in testing, a 114-byte payload is denoted as 118 bytes per payload length? */
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

  struct websocket_response *verif = wsutil_parse_response (websocket_key, server_response);

  if (verif == NULL)
    {
      debug_print ("likely got misformatted/non-HTTP methodline");
      return false;
    }
  else if (!verif->is_101)
    {
      debug_print ("HTTP method wasn't 101");
      return false;
    }
  else if (!verif->is_upgrade_hdr)
    {
      debug_print ("invalid Upgrade header sent");
      return false;
    }
  else if (!verif->is_connection_hdr)
    {
      debug_print ("invalid Connection header sent");
      return false;
    }
  else if (!verif->is_wskey_hash)
    {
      debug_print ("mismatching Sec-WebSocket-Accept sent");
      return false;
    }
  else if (verif->is_extensions)
    {
      debug_print ("Sec-WebSocket-Extensions sent, but as of now unsupported");
      return false;
    }
  else if (verif->is_protocols)
    {
      debug_print ("Sec-WebSocket-Protocol sent, but as of now unsupported");
      return false;
    }

  free (verif);
  free (server_response);
  free ((void *)websocket_key);
  return true;
} 
