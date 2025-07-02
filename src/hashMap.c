#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hashMap.h"

uint32_t fnv1a(const char* data, size_t len) {
	uint32_t hash = 2166136261u;

    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)data[i];
        hash *= 16777619u;
    }

    return hash;
}

HashMapEntryData* get(HashMap *map, char *key) {
	uint32_t hash = fnv1a(key, strnlen(key, KEY_SIZE));

	HashMapEntry* entry = (HashMapEntry*)(map->entries + (sizeof(HashMapEntry) * hash));

	if (entry == NULL) {
		return NULL;
	}
	
	HashMapEntry* runner = entry;

	while (runner != NULL && strncmp(runner->key, key, KEY_SIZE) != 0) {
		runner = runner->next;
	}

	if (runner == NULL) {
		return NULL;
	}

	return &runner->value;
}
