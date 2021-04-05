#include <ctype.h>
#include "websocket.h"
#include "dict.h"
#include "sha1.h"
#include "base64.h"

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

size_t
wsutil_strarray_len (const char **strings)
{
  size_t len = 0;
  for (; strings[len++];);
  return len;
}

bool
wsutil_strncmp_lower (const char *cmp1, const char *cmp2, size_t n)
{
  for (size_t i = 0; i < n; ++i)
      if (tolower (cmp1[i]) != tolower (cmp2[i]))
        return true;
  return false;
}

dict_t*
wsutil_parse_headers (const char ** hdr_mask, const char * data)
{
  size_t crlf_idx = wsutil_read_until (data, HEADER_ENDL);

  /* read preliminary GET methodline */
  if (crlf_idx == -1)
    {
      debug_print ("some weird ass request header lmao, no CRLFs???");
      return NULL;
    }
  else if (!!strncmp (hdr_mask[0], data, crlf_idx))
    {
      debug_print ("mismatching request methods");
      return NULL;
    }

  /* parse over each CRLF-terminated line */
  size_t cur_idx, hdr_mask_len,
          key_length, value_length;
  char *value;
  const char *cur_hdr_mask;

  dict_t *dict = dict_new (128);
  dict_add_item (dict, "http", (void *)"101");

  while ( crlf_idx != -1)
    {
      crlf_idx += 2;
      if ( (cur_idx = wsutil_read_until (&data[crlf_idx], HEADER_ENDL)) == -1)
        break;
      else if (!cur_idx)
        break;

      for (size_t hdr_idx = 0; cur_hdr_mask = hdr_mask[hdr_idx]; ++hdr_idx)
        {
          hdr_mask_len = strlen (cur_hdr_mask);
          if (wsutil_strncmp_lower (&data[crlf_idx], cur_hdr_mask, hdr_mask_len))
            continue;
          key_length = wsutil_read_until (&data[crlf_idx], ":");
          
          /* +0x2 offset assuming `:<SPACE>` between key/value */
          value_length = wsutil_read_until (&data[crlf_idx + key_length + 2], HEADER_ENDL);
          value = (char *)calloc (sizeof (char), 64);
          memcpy (value, &data[crlf_idx + key_length + 2], value_length);
          value[value_length] = '\0';

          dict_add_item (dict, cur_hdr_mask, value);
        }

      crlf_idx += cur_idx;
    }
  return dict;
}

bool
wsutil_verify_wskey (const char *ws_key, const char *server_key)
{
  char concat[strlen (ws_key) + strlen (WS_MAGIC) + 1] = {0};
  char *sha1_concat, *b64_sha1_concat;
  strcat ((char *)memcpy (concat, ws_key, strlen (ws_key)), WS_MAGIC);
  sha1_concat = sha1_create_digest (concat);
  b64_sha1_concat = (char *)calloc (sizeof (char), BASE64_ENCODE_LEN (SHA_DIGEST_LENGTH) + 1);
  base64_encode (b64_sha1_concat, sha1_concat, SHA_DIGEST_LENGTH);
  int res = strcmp (b64_sha1_concat, server_key);
  free (b64_sha1_concat);
  free (sha1_concat);
  return !res;
}

struct websocket_response*
wsutil_parse_response (const char *ws_key, const char *server_response)
{
  // visual cancer
  struct websocket_response *ws_res = (struct websocket_response *)malloc (sizeof (struct websocket_response));
  const char *hdr_mask[] = {
    "HTTP/1.1 101 Switching Protocols",  /* special */
    "upgrade",
    "connection",
    "sec-websocket-accept",
    "sec-websocket-extensions",
    "sec-websocket-protocol",
    0  /* term */
    };
  dict_t *dict = wsutil_parse_headers (hdr_mask, server_response);

  if (dict == NULL)
    return NULL;

  ws_res->is_101 = !strcmp ((const char *)dict_get_item (dict, "http"), "101");
  ws_res->is_upgrade_hdr = !strcmp ((const char *)dict_get_item (dict, "upgrade"), "websocket");
  ws_res->is_connection_hdr = !strcmp ((const char *)dict_get_item (dict, "connection"), "upgrade");
  ws_res->is_extensions = dict_get_item (dict, "sec-websocket-extensions") != NULL;
  ws_res->is_protocols = dict_get_item (dict, "sec-websocket-protocol") != NULL;
  ws_res->is_wskey_hash = wsutil_verify_wskey (ws_key, (const char*)dict_get_item (dict, "sec-websocket-accept"));

  dict_free (dict);
  return ws_res;
}

void
wsutil_print_message (websocket_msg_t msg)
{
  printf ("is final: %s, rsv bits: %d %d %d, opcode: %x\n"
          "is masked: %s, payload length: %d, extended length: %d\n"
          "masking key: %d\n data: %s\n", msg.fin? "yes": "no", !!msg.rsv1, !!msg.rsv2, !!msg.rsv3,
          msg.opcode, msg.mask? "yes": "no", msg.payload_length, msg.ext_payload_length,
          msg.masking_key, msg.payload_data);
}