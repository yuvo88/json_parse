#include "hashMap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t fnv1a(const char *data, size_t len) {
  uint32_t hash = 2166136261u;

  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)data[i];
    hash *= 16777619u;
  }

  return hash;
}

void replaceHashmapEntryValue(HashMapEntry *source, HashMapEntry *destination) {
  destination->value = source->value;
  destination->next = source->next;
}

ReturnCodes hashmapGetByKey(HashMap *map, const char *key,
                            FieldData *returnMap) {
  assert(key != NULL);
  assert(map != NULL);
  assert(map->entries != NULL);
  assert(returnMap != NULL);
  uint32_t hash = fnv1a(key, strnlen(key, KEY_SIZE));

  HashMapEntry *entry = map->entries[hash];

  if (entry == NULL) {
    return KEY_NOT_FOUND;
  }
  HashMapEntry *runner = entry;

  while (runner != NULL && strncmp(runner->key, key, KEY_SIZE) != 0) {
    runner = runner->next;
  }

  if (runner == NULL) {
    return KEY_NOT_FOUND;
  }
  *returnMap = runner->value;
  return SUCCESS;
}

ReturnCodes hashmapSetFieldData(HashMap *map, HashMapEntry *value) {
  assert(map != NULL);
  assert(map->entries != NULL);
  assert(value != NULL);
  char *key = value->key;
  assert(key != NULL);
  uint32_t hash = fnv1a(key, strnlen(key, KEY_SIZE));

  HashMapEntry *head = map->entries[hash];
  if (head == NULL) {
    map->entries[hash] = value;
    map->count++;
    return SUCCESS;
  }
  int cond = 1;
  HashMapEntry *runner = head;
  while (strncmp(runner->key, key, KEY_SIZE) != 0) {
    if (runner->next != NULL) {
      cond = 0;
      break;
    }
    runner = runner->next;
  }
  if (!cond) {
    runner->next = value;
    map->count++;
    return SUCCESS;
  }
  replaceHashmapEntryValue(value, runner);
  return SUCCESS;
}
