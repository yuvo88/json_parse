#include "arena.h"
#include "hashmap.h"
#include <stdlib.h>

void freeEntryValue (EntryValue* entryValue) {
    switch (entryValue->type) {
    case HASHMAP: freeHashmap ((Hashmap*)entryValue->value); break;
    case LIST: freeList ((List*)entryValue->value); break;
    case SUPER_PRIMITIVE:
        freeSuperPrimitive ((SuperPrimitive*)entryValue->value);
        break;
    case NULL_VALUE: break;
    }
    free (entryValue);
}

EntryValue* createEntryValue (Arena* arena, void* value, EntryType type) {
    EntryValue* entryValue = (EntryValue*)(arenaMalloc (arena, sizeof (EntryValue)));
    entryValue->value      = value;
    entryValue->type       = type;
    return entryValue;
}
