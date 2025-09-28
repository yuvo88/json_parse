#include "hashmap.h"
#include "superPrimitive.h"
#include <stdint.h>
#include <stdlib.h>


Hashmap* createHashmap () {
    HashmapEntry** entries =
    (HashmapEntry**)(calloc (ARRAY_INITIAL_LENGTH, sizeof (HashmapEntry*)));
    Hashmap* hashmap = (Hashmap*)(malloc (sizeof (Hashmap)));
    hashmap->length  = ARRAY_INITIAL_LENGTH;
    hashmap->entries = entries;
    return hashmap;
}

void freeHashmap (Hashmap* hashmap) {
    for (uint32_t i = 0; i < hashmap->length; i++) {
        HashmapEntry* entry = (HashmapEntry*)(*(hashmap->entries + i));
        if (entry == NULL) {
            continue;
        }
        switch (entry->type) {
        case HASHMAP: freeHashmap ((Hashmap*)entry->value); break;
        case LIST: break;
        case SUPER_PRIMITIVE:
            freeSuperPrimitive ((SuperPrimitive*)entry->value);
            break;
        }
        freeSuperPrimitive ((SuperPrimitive*)entry->key);
        free (entry);
    }
    free (hashmap->entries);
    free (hashmap);
}
