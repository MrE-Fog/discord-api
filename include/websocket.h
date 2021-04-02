#ifndef __WEBSOCKET_H
#define __WEBSOCKET_H

#include <stdbool.h>
#include <string.h>
#include "common.h"

#define GENERIC_HEADER_SIZE 65536
#define HEADER_VERSION      "HTTP/1.1"
#define HEADER_ENDL         "\r\n"
#define PARSE_TOKSIZE       64

typedef void (*write_function_t)(char*);
typedef void (*read_function_t)(char*, size_t);

struct websocket_response {
  bool is_101 : 1;
  bool is_upgrade_hdr : 1;
  bool is_connection_hdr : 1;
  bool is_wskey_hash : 1;
  bool is_extensions : 1;
  bool is_protocols : 1;
};  /* pray and hope this optimizes to a bool */

bool ws_upgrade_connection (int, const char*, const char*,
    write_function_t, read_function_t);

size_t
wsutil_read_until (const char *data, const char *cmp)
{
  size_t i = 0,
         data_len = strlen (data),
         cmp_len = strlen (cmp);
  if (cmp_len > data_len)
    return -1;
  for (; i < data_len - cmp_len + 1; ++i)
      if (!strncmp (&data[i], cmp, cmp_len))
        return i;
  return -1;
}

bool
wsutil_parse_headers (char *** out, const char ** hdr_mask, const char * data)
{
  char current_token[PARSE_TOKSIZE];
  size_t req_hdr_idx = wsutil_read_until (data, HEADER_ENDL);
  if (req_hdr_idx == -1)
    {
      debug_print ("some weird ass request header lmao, no CRLFs???");
      return false;
    }
  else if (!!strcmp (hdr_mask[0], data))
    {
      debug_print ("mismatching request methods");
      return false;
    }
}

struct websocket_response*
wsutil_parse_response (const char *ws_key, const char *server_response)
{
  // visual cancer
  struct websocket_response *ws_res = (struct websocket_response *)malloc (sizeof (struct websocket_response));
  const char **hdr_mask = {
    "HTTP/1.1 101 Switching Protocols",  /* special */
    "upgrade",
    "connection",
    "sec-websocket-accept",
    "sec-websocket-extensions",
    "sec-websocket-protocol", 0
    };
  char **headers;
  wsutil_parse_headers (&headers, hdr_mask, server_response);
}

#endif  /* __WEBSOCKET_H */
