#include "hashMap.h"
#include <assert.h>
#include <bits/pthreadtypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t fnv1a(const char *data, size_t len) {
  uint32_t hash = 2166136261u;

  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)data[i];
    hash *= 16777619u;
  }

  return hash;
}

int getFromHashmap(HashMap *map, const char *key, HashMapEntryData *returnMap) {
  if (key == NULL) {
    return INVALID_PARAMETER_ERROR;
  }

  if (map->entries == NULL) {
    return INVALID_PARAMETER_ERROR;
  }
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
