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
wsutil_read_until (const char *data, const char *cmp);
bool
wsutil_parse_headers (char *** out, const char ** hdr_mask, const char * data);
struct websocket_response*
wsutil_parse_response (const char *ws_key, const char *server_response);

#endif  /* __WEBSOCKET_H */
