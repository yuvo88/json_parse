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

  return hash & (HASH_ARRAY_LENGTH - 1);
}

void replaceHashmapEntryValue(HashMapEntry *source, HashMapEntry *destination) {
  destination->value = source->value;
  destination->next = source->next;
}

HashMap createHashmap() {
  HashMapEntry **entries =
      (HashMapEntry **)(calloc(HASH_ARRAY_LENGTH, sizeof(HashMapEntry *)));
  HashMap hashmap = {.count = 0, .entries = entries};
  return hashmap;
}

void freeHashmap(HashMap hashmap) {
  for (int i = 0; i < HASH_ARRAY_LENGTH; i++) {
    if (hashmap.entries[i] == NULL) {
      continue;
    }
    free(hashmap.entries[i]);
  }
  free(hashmap.entries);
}

ReturnCodes hashmapDeleteByKey(HashMap *map, const char *key) {
  assert(key != NULL);
  assert(map != NULL);
  assert(map->entries != NULL);
  uint32_t hash = fnv1a(key, strnlen(key, KEY_SIZE));
  HashMapEntry *entry = map->entries[hash];

  if (entry == NULL) {
    return KEY_NOT_FOUND;
  }
  HashMapEntry *runner = entry;
  HashMapEntry *previous = NULL;
  int cond = 1;
  while (strncmp(runner->key, key, KEY_SIZE) != 0) {
    if (runner == NULL) {
      cond = 0;
      break;
    }
    runner = runner->next;
    previous = runner;
  }
  if (!cond) {
    return KEY_NOT_FOUND;
  }
  if (previous == NULL) {
    map->entries[hash] = NULL;
    free(runner);
    map->count--;
    return SUCCESS;
  }
  previous->next = runner->next;
  free(runner);
  map->count--;
  return SUCCESS;
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

ReturnCodes hashmapSetFieldData(HashMap *map, const char *key,
                                FieldData value) {
  assert(map != NULL);
  assert(map->entries != NULL);
  assert(key != NULL);
  HashMapEntry *entry = (HashMapEntry *)(malloc(sizeof(HashMapEntry)));
  uint32_t hash = fnv1a(key, strnlen(key, KEY_SIZE));
  memcpy(&entry->value, &value, sizeof(FieldData));

  HashMapEntry *head = map->entries[hash];
  if (head == NULL) {
    map->entries[hash] = entry;
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
    runner->next = entry;
    map->count++;
    return SUCCESS;
  }
  replaceHashmapEntryValue(entry, runner);
  return SUCCESS;
}
