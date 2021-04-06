#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "grammar.h"

json_array_t*
json_parse_array (const char *string)
{
  json_array_t *array = (json_array_t *)malloc (sizeof (json_array_t));
  return array;
}

void
json_print_error (const char *nature, const char *string)
{
  printf ("%s error: %s\n", nature, string);
}

size_t
json_parse_string (json_string_t* out, const char *string)
{
  json_string_t parsed = (json_string_t)calloc (DEFAULT_STRINGSIZE, sizeof (char));

  bool escape_seq = false;

  for (size_t idx = 0;; ++idx)
    {
      switch (string[idx])
        {
          case (ESCAPE):
          {
            if (escape_seq)
              {
                strcat ((char *)parsed, "\\");
              }
            else
              escape_seq = true;
            break;
          }
          case (QUOTE_MARK):
          {
            if (escape_seq)
              {
                strcat ((char *)parsed, "\"");
              }
            else
              {
                *out = parsed;
                return idx + 1;
              }
            break;
          }
          default:
          {
            strncat ((char *)parsed, &string[idx], 1);
          }
        }
    }
}

json_item_t*
json_parse_item (const char *string)
{
  json_item_t *items = (json_item_t *)calloc (DEFAULT_BINSIZE, sizeof (json_item_t));
  json_item_t current_token;

  parse_state_t pstate = {
    .expecting_key = true,
    .object_literals = 1,
    .array_literals = 0
    };

  size_t item_count = 0;

  for (size_t idx = 0; idx < strlen (string);)
    {
      switch (string[idx])
        {
          case (QUOTE_MARK):
          {
            if (pstate.expecting_key)
              idx += json_parse_string (&current_token.key, &string[idx+1]) + 1;
            else
              {
                if (current_token.key == NULL)
                  {
                    json_print_error ("parse", "expecting value, but no key parsed");
                    goto on_error;
                  }
                  idx += json_parse_string (&current_token.value.as_string, &string[idx+1]) + 1;
                  pstate.expecting_key = true;
              }
            break;
          }
          case (NAME_SEP):
          {
            if (!pstate.expecting_key)
              {
                json_print_error ("parse", "multiple name separators found");
                goto on_error;
              }
            pstate.expecting_key = false;
            idx++;
            break;
          }
          case (VALUE_SEP):
          {
            memcpy (&items[item_count++], (void *)&current_token, sizeof (json_item_t));
            memset (&current_token, 0, sizeof (json_item_t));
            if (item_count > DEFAULT_BINSIZE)
              {
                json_print_error ("internal", "exceeded default binsize");
                goto on_error;
              }
            pstate.expecting_key = true;
            idx++;
            break;
          }
          case (END_OBJECT):
          {
            if (pstate.expecting_key)
              {
                memcpy (&items[item_count++], (void *)&current_token, sizeof (json_item_t));
                memset (&current_token, 0, sizeof (json_item_t));
              }
            if (pstate.object_literals - 1 == 0)
              goto realloc_proc;
          }
          default:
          {
            idx++;
            break;
          }
        }
    }
realloc_proc:
  realloc (items, sizeof (json_item_t) * item_count);
  return items;
on_error:
  return NULL;
}

json_generic_t
json_loadstring (const char *string)
{
  json_generic_t generic = {0};
  switch (string[0])
    {
      case (BEGIN_ARRAY):
        generic.as_array = json_parse_array (&string[1]);
      case (BEGIN_OBJECT):
        generic.as_object = json_parse_item (&string[1]);
      default:
        return generic;
    }
  return generic;
}

json_item_t*
json_get_nth_item (json_item_t *json, size_t n)
{
  return &json[n];
}

int
main (void)
{
  json_item_t *json = json_loadstring ("{\"key\": \"data\", \"another key\": \"more data\"}").as_object;
  printf ("json[%s] = '%s'\n", (&json[1])->key, (&json[1])->value.as_string);
  return EXIT_SUCCESS;
}
