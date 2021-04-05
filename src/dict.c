#include "dict.h"
#include "common.h"

dict_t*
dict_new (size_t initial_capacity)
{
  debug_print ("creating new dictionary");

  if (!initial_capacity)
    return NULL;

  dict_t *dict = (dict_t *)malloc (sizeof (dict_t));
  dict->array = (dict_item_t *)calloc (initial_capacity, sizeof (dict_item_t));
  dict->capacity = initial_capacity;

  for (size_t i = 0; i < initial_capacity; ++i)
    dict->array[i].in_use = false;

  return dict;
}

bool
dict_free (dict_t *dict)
{
  debug_print ("freeing dictionary");
  free (dict->array);
  free (dict);
}

size_t
dict_hash_key (dict_t *dict, const char *key)
{
  size_t weighted_sum = 0;
  for (size_t i = 0; i < strlen (key); ++i)
    weighted_sum += (i + 1) * key[i];
  size_t res = weighted_sum & (dict->capacity - 1);
  return res;
}

size_t
dict_add_item (dict_t *dict, const char *key, void *data)
{
  dict_item_t dict_item = {
      .in_use = true,
      .key = key,
      .value = data
    };
  debug_print ("adding item to dictionary");
  size_t hash = dict_hash_key (dict, key);
  if (dict->array[hash].in_use)
    debug_print ("woops, collision, not handling this");
  memcpy (&dict->array[hash], &dict_item, sizeof (dict_item_t));
  return hash;
}

bool
dict_remove_item (dict_t *dict, const char *key)
{
  debug_print ("removing dictionary item");
  size_t hash = dict_hash_key (dict, key);
  if (!dict->array[hash].in_use)
    return false;
  dict->array[hash].in_use = false;
  return true;
}

void*
dict_get_item (dict_t *dict, const char *key)
{
  size_t hash = dict_hash_key (dict, key);
  if (!dict->array[hash].in_use)
    return NULL;
  else if (strcmp (dict->array[hash].key, key))
    return NULL;
  return dict->array[hash].value;
}