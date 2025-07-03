#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "hashMap.h"


int main() {
	HashMapEntryData data = {.data = "sldfkjsdflkj", .type = VARCHAR};
	HashMapEntry entry = {.key = "something", .next = NULL, .value = data}; 
	HashMapEntry** entries = (HashMapEntry**)(malloc(sizeof(int) * UINT32_MAX));
	uint32_t location = fnv1a("something", 9);
	entries[location] = &entry;
	HashMap hashmap = { .count = 1, .entries = entries};
	HashMapEntryData* some = getFromHashmap(&hashmap, "something");
	printf("returned_data: %s\n", some->data);
	free(entries);
    return 0;
}
