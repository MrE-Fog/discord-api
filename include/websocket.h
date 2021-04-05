#ifndef __WEBSOCKET_H
#define __WEBSOCKET_H

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "common.h"

#define GENERIC_HEADER_SIZE 65536
#define HEADER_VERSION      "HTTP/1.1"
#define HEADER_ENDL         "\r\n"
#define WS_MAGIC            "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

typedef void (*write_function_t)(char*);
typedef void (*read_function_t)(char*, size_t);

typedef struct {
  bool fin : 1;
  bool rsv1 : 1;
  bool rsv2 : 1;
  bool rsv3 : 1;
  unsigned short opcode : 4;

  bool mask : 1;
  unsigned short payload_length : 7;

  uint64_t ext_payload_length;
  uint32_t masking_key;

  char *payload_data;
} websocket_msg_t;

struct websocket_response {
  bool is_101 : 1;
  bool is_upgrade_hdr : 1;
  bool is_connection_hdr : 1;
  bool is_wskey_hash : 1;
  bool is_extensions : 1;
  bool is_protocols : 1;
};  /* pray and hope this optimizes to a bool */

bool
ws_upgrade_connection (int, const char*, const char*,
    write_function_t, read_function_t);

void
wsutil_print_message (websocket_msg_t msg);

websocket_msg_t
ws_read_message (const char *msg);

size_t
wsutil_read_until (const char *data, const char *cmp);
bool
wsutil_parse_headers (char *** out, const char ** hdr_mask, const char * data);

struct websocket_response*
wsutil_parse_response (const char *ws_key, const char *server_response);

#endif  /* __WEBSOCKET_H */
