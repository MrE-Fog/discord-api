#include <ctype.h>
#include "websocket.h"


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

bool
wsutil_parse_headers (char *** out, const char ** hdr_mask, const char * data)
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
  size_t cur_idx, hdr_mask_len;
  const char *cur_hdr_mask;

  char **parsed_hdrs = (char **)calloc (sizeof (char *), wsutil_strarray_len (hdr_mask));

  while ( crlf_idx != -1)
    {
      crlf_idx += 2;
      if ( (cur_idx = wsutil_read_until (&data[crlf_idx], HEADER_ENDL)) == -1)
        break;
      else if (!cur_idx)
        break;
      // &data[crlf_idx] to &data[crlf_idx+cur_idx] = line
      
      for (size_t hdr_idx = 0; cur_hdr_mask = hdr_mask[hdr_idx]; ++hdr_idx)
        {
          hdr_mask_len = strlen (cur_hdr_mask);
          if (wsutil_strncmp_lower (&data[crlf_idx], cur_hdr_mask, hdr_mask_len))
            continue;
          printf ("%.*s\n", cur_idx, &data[crlf_idx]);
        }

      crlf_idx += cur_idx;
    }

  free (parsed_hdrs);
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
    "sec-websocket-protocol", 0
    };
  char **parsed_hdrs;
  wsutil_parse_headers (&parsed_hdrs, hdr_mask, server_response);
  free (ws_res);
}