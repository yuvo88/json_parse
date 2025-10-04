#include "hashmap.h"
#include "superPrimitive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHashmapEntryPrivate (HashmapEntry* entry, uint32_t amount);
void printHashmapPrivate (Hashmap* hashmap, uint32_t amount);
void printHashmapEntryValuePrivate (HashmapEntry* entry, uint32_t spaceAmount);
void freeHashmapEntry (HashmapEntry* entry);
void adjustEntryArrayLength (Hashmap* hashmap);
void setKeyAndValuePrivate (Hashmap* hashmap,
SuperPrimitive* key,
void* value,
HashmapEntryType type,
uint32_t hashOriginal);

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

void setKeyAndValue (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type) {
    uint32_t hashOriginal = fnv1 (key);
    setKeyAndValuePrivate (hashmap, key, value, type, hashOriginal);
}

void setKeyAndValuePrivate (Hashmap* hashmap,
SuperPrimitive* key,
void* value,
HashmapEntryType type,
uint32_t hashOriginal) {
    HashmapEntry* entry = (HashmapEntry*)malloc (sizeof (HashmapEntry));
    entry->value        = value;
    entry->type         = type;
    entry->key          = key;
    entry->next         = NULL;
    entry->hashOriginal = hashOriginal;
    uint32_t hash       = hashOriginal & (hashmap->length - 1);
    HashmapEntry* head  = hashmap->entries[hash];
    if (head == NULL) {
        hashmap->entries[hash] = entry;
        return;
    }

    HashmapEntry* runner = head;
    int found            = 1;
    int nextListLength   = 0;
    while (1) {
        if (runner->key->size == key->size &&
        memcmp (runner->key->value, key->value, runner->key->size) == 0) {
            break;
        }
        if (runner->next == NULL) {
            found = 0;
            break;
        }
        nextListLength++;
        runner = head->next;
    }
    if (found) {
        runner->value = value;
        runner->type  = type;
        return;
    }

    runner->next = entry;
    if (nextListLength > 3) {
        adjustEntryArrayLength (hashmap);
    }
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

void printHashmap (Hashmap* hashmap) {
    printHashmapPrivate (hashmap, 0);
    printf ("\n");
}

void printSpaces (uint32_t amount) {
    for (uint32_t i = 0; i < amount; i++) {
        printf (" ");
    }
}

void printHashmapEntry (HashmapEntry* entry) {
    printHashmapEntryPrivate (entry, 0);
    printf ("\n");
}

void printHashmapEntryValue (HashmapEntry* entry) {
    printHashmapEntryValuePrivate (entry, 0);
    printf ("\n");
}

void printHashmapPrivate (Hashmap* hashmap, uint32_t spaceAmount) {
    int firstValue        = 1;
    uint32_t insideAmount = spaceAmount + 4;
    printf ("{\n");
    for (uint32_t i = 0; i < hashmap->length; i++) {
        HashmapEntry* entry = hashmap->entries[i];
        if (entry == NULL) {
            continue;
        }

        if (!firstValue) {
            printf (",\n");
        } else {
            firstValue = 0;
        }
        printSpaces (insideAmount);
        printHashmapEntryPrivate (entry, spaceAmount);
        HashmapEntry* runner = entry->next;
        while (runner != NULL) {
            printf (",\n");
            printSpaces (insideAmount);
            printHashmapEntryPrivate (runner, spaceAmount);
            runner = runner->next;
        }
    }
    printf ("\n");
    printSpaces (spaceAmount);
    printf ("}");
}

void printHashmapEntryValuePrivate (HashmapEntry* entry, uint32_t spaceAmount) {
    switch (entry->type) {
    case SUPER_PRIMITIVE:
        printSuperPrimitive ((SuperPrimitive*)entry->value);
        break;
    case HASHMAP:
        printHashmapPrivate ((Hashmap*)entry->value, spaceAmount + 4);
        break;
    case LIST: break;
    }
}

void printHashmapEntryPrivate (HashmapEntry* entry, uint32_t spaceAmount) {
    printSuperPrimitive (entry->key);
    printf (":");
    printf (" ");
    printHashmapEntryValuePrivate (entry, spaceAmount);
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

void adjustEntryArrayLength (Hashmap* hashmap) {
    uint32_t newLength = hashmap->length * 2;
    HashmapEntry** entries =
    (HashmapEntry**)(calloc (newLength, sizeof (HashmapEntry*)));
    for (uint32_t i = 0; i < hashmap->length; i++) {
        HashmapEntry* entry = hashmap->entries[i];

        if (entry == NULL) {
            continue;
        }
        uint32_t hash = entry->hashOriginal & (newLength - 1);
        setKeyAndValuePrivate (
        hashmap, entry->key, entry->value, entry->type, entry->hashOriginal);
    }

    free (hashmap->entries);
    hashmap->entries = entries;
    hashmap->length = newLength;
}
