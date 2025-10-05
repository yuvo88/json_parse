#include "hashmap.h"
#include "superPrimitive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeHashmapEntry (HashmapEntry* entry);
void expandEntryArray (Hashmap* hashmap);
int setHashmapEntryWithMask (HashmapEntry** entries, HashmapEntry* entry, uint32_t mask

);

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
        freeHashmapEntry (entry);
    }
    free (hashmap->entries);
    free (hashmap);
}

void freeHashmapEntry (HashmapEntry* entry) {
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

void setHashmapEntry (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type) {
    HashmapEntry* entry = (HashmapEntry*)malloc (sizeof (HashmapEntry));
    entry->value        = value;
    entry->type         = type;
    entry->key          = key;
    entry->next         = NULL;
    entry->originalHash = fnv1 (key);
    int nextListLegnth =
    setHashmapEntryWithMask (hashmap->entries, entry, hashmap->length - 1);
    if (nextListLegnth >= 3) {
        expandEntryArray (hashmap);
    }
}

int setHashmapEntryWithMask (HashmapEntry** entries, HashmapEntry* entry, uint32_t mask

) { // TODO: This function returns a value and I don't think it should
    uint32_t hash      = entry->originalHash & mask;
    HashmapEntry* head = entries[hash];
    if (head == NULL) {
        entries[hash] = entry;
        return 0;
    }

    HashmapEntry* runner   = head;
    HashmapEntry* previous = NULL;
    int found              = 1;
    int nextListLength     = 1;
    while (1) {
        if (runner->key->size == entry->key->size &&
        memcmp (runner->key->value, entry->key->value, runner->key->size) == 0) {
            break;
        }
        if (runner->next == NULL) {
            found = 0;
            break;
        }
        nextListLength++;
        runner   = runner->next;
        previous = runner;
    }
    if (!found) {
        runner->next = entry;
        return nextListLength;
    }

    if (previous == NULL) {
        entries[hash] = entry;
        freeHashmapEntry (runner);
        return 0;
    }

    previous->next = entry;
    freeHashmapEntry (runner);
    return nextListLength;
}
HashmapReturnCodes deleteEntryByKey (Hashmap* hashmap, SuperPrimitive* key) {
    uint32_t hash        = fnv1 (key) & (hashmap->length - 1);
    HashmapEntry* runner = hashmap->entries[hash];
    if (runner == NULL) {
        return KEY_NOT_FOUND;
    }
    HashmapEntry* previous = NULL;
    int found              = 1;
    while (1) {
        if (runner->key->size == key->size &&
        memcmp (runner->key->value, key->value, runner->key->size) == 0) {
            break;
        }
        if (runner == NULL) {
            found = 0;
            break;
        }
        previous = runner;
        runner   = runner->next;
    }

    if (!found) {
        return KEY_NOT_FOUND;
    }
    if (previous == NULL) {
        hashmap->entries[hash] = NULL;
        freeHashmapEntry (runner);
        return SUCCESS;
    }

    previous->next = runner->next;
    freeHashmapEntry (runner);
    return SUCCESS;
}


HashmapReturnCodes
getValueByKey (Hashmap* hashmap, SuperPrimitive* key, HashmapEntry* returnValue) {
    uint32_t hash       = fnv1 (key) & (hashmap->length - 1);
    HashmapEntry* entry = hashmap->entries[hash];
    if (entry == NULL) {
        return KEY_NOT_FOUND;
    }


    HashmapEntry* runner = entry;

    while (runner != NULL) {
        if (runner->key->size == key->size &&
        memcmp (runner->key->value, key->value, runner->key->size) == 0) {
            break;
        }
        runner = runner->next;
    }
    if (runner == NULL) {
        return KEY_NOT_FOUND;
    }

    memcpy (returnValue, runner, sizeof (HashmapEntry));
    return SUCCESS;
}

void expandEntryArray (Hashmap* hashmap) {
    uint32_t newLength = hashmap->length * 2;
    HashmapEntry** entries =
    (HashmapEntry**)(calloc (newLength, sizeof (HashmapEntry*)));
    for (uint32_t i = 0; i < hashmap->length; i++) {
        HashmapEntry* entry = hashmap->entries[i];

        if (entry == NULL) {
            continue;
        }
        HashmapEntry* runner = entry->next;
        entry->next          = NULL;
        setHashmapEntryWithMask (entries, entry, newLength - 1);

        while (runner != NULL) {
            HashmapEntry* next = runner->next;
            runner->next       = NULL;
            setHashmapEntryWithMask (entries, runner, newLength - 1);
            runner = next;
        }
    }

    free (hashmap->entries);
    hashmap->entries = entries;
    hashmap->length  = newLength;
}
