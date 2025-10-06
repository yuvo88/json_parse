#include "hashmap.h"
#include <stdlib.h>

void freeEntryValue (EntryValue* entryValue) {
    switch (entryValue->type) {
    case HASHMAP: freeHashmap ((Hashmap*)entryValue->value); break;
    case LIST: break;
    case SUPER_PRIMITIVE:
        freeSuperPrimitive ((SuperPrimitive*)entryValue->value);
        break;
    }
    free (entryValue);
}

EntryValue* createEntryValue (void* value, EntryType type) {
    EntryValue* entryValue = (EntryValue*)(malloc (sizeof (EntryValue)));
    entryValue->value      = value;
    entryValue->type       = type;
    return entryValue;
}
