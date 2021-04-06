#ifndef __JSON_H
#define __JSON_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define DEFAULT_BINSIZE     32
#define DEFAULT_STRINGSIZE  128
#define REALLOC_INCREMENT   32

struct json_item;
typedef struct json_item json_item_t;

struct json_array;
typedef struct json_array json_array_t;

typedef const char*     json_string_t;
typedef bool      json_bool_t;
typedef uint64_t  json_int_t;
typedef void*     json_null_t;
typedef double    json_double_t;

typedef union json_generic {
  json_item_t*  as_object;
  json_array_t* as_array;
} json_generic_t;

struct json_array
{
  union array_item
  {
    json_string_t string;
    json_bool_t   boolean;
    json_int_t    integer;
    json_null_t   null;
    json_double_t decimal;
    json_item_t*  object;
    json_array_t* array;
  } *items;
};

struct json_item
{
  const char *key;
  union
  {
    json_string_t as_string;
    json_bool_t   as_boolean;
    json_int_t    as_integer;
    json_null_t   as_null;
    json_double_t as_decimal;
    json_item_t*  as_object;
    json_array_t* as_array;
  } value;
};

typedef struct {
  bool expecting_key  :1;
  int64_t object_literals;
  int64_t array_literals;
} parse_state_t;

json_generic_t
json_loadstring (const char *string);

json_array_t*
json_parse_array (const char *string);

json_item_t*
json_parse_item (const char *string);

void
json_free_array (json_array_t *array);

void
json_free_item (json_item_t *item);

void
json_print_error (const char *nature, const char *string);

size_t
json_parse_string (json_string_t* out, const char *string);

#endif  /* __JSON_H */