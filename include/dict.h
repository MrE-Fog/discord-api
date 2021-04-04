#ifndef __DICT_H
#define  __DICT_H

#include <stdlib.h>
#include <stdbool.h>
#define DICT_REALLOC_SIZE 32

typedef struct {
  bool in_use;
  const char *key;
  void *value;
} dict_item_t;

typedef struct {
  dict_item_t *array;
  size_t capacity;
} dict_t;

dict_t*
dict_new (size_t initial_capacity);

bool
dict_free (dict_t * dict);

size_t
dict_add_item (dict_t * dict, const char * key, void * data);

bool
dict_remove_item (dict_t * dict, const char * key);

void*
dict_get_item (dict_t * dict, const char * key);

size_t
dict_hash_key (dict_t * dict, const char * key);

#endif  /* __DICT_H */