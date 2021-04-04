#include <ctype.h>
#include "websocket.h"
#include "dict.h"

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
      return false;
    }
  else if (!!strncmp (hdr_mask[0], data, crlf_idx))
    {
      debug_print ("mismatching request methods");
      return false;
    }

  /* parse over each CRLF-terminated line */
  size_t cur_idx, hdr_mask_len,
          key_length, value_length;
  char key[64];
  char value[64];
  const char *cur_hdr_mask;

  dict_t *dict = dict_new (128);
  dict_add_item (dict, "http", "101");

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
          memcpy (key, &data[crlf_idx], key_length);
          key[key_length] = '\0';
          
          /* +0x2 offset assuming `:<SPACE>` between key/value */
          value_length = wsutil_read_until (&data[crlf_idx + key_length + 2], HEADER_ENDL);
          memcpy (value, &data[crlf_idx + key_length + 2], value_length);
          value[value_length] = '\0';

          dict_add_item (dict, key, value);
        }

      crlf_idx += cur_idx;
    }

  return dict;
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
  dict_t *parsed = wsutil_parse_headers (hdr_mask, server_response);
  free (ws_res);
}
