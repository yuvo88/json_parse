#include "hashmap.h"
#include "superPrimitive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHashmapEntry (HashmapEntry* entry, uint32_t amount);
void printHashmapPrivate (Hashmap* hashmap, uint32_t amount);
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

void setKeyAndValue (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type) {
    HashmapEntry* entry = (HashmapEntry*)malloc (sizeof (HashmapEntry));
    entry->value        = value;
    entry->type         = type;
    entry->key          = key;
    entry->next         = NULL;
    uint32_t hash       = fnv1 (key, hashmap->length - 1);
    HashmapEntry* head  = hashmap->entries[hash];
    if (head == NULL) {
        hashmap->entries[hash] = entry;
        return;
    }

    HashmapEntry* runner = head;
    int found            = 1;
    while (1) {
        if (runner->key->size == key->size &&
        memcmp (runner->key->value, key->value, runner->key->size) == 0) {
            break;
        }
        if (runner->next == NULL) {
            found = 0;
            break;
        }
        runner = head->next;
    }
    if (found) {
        runner->value = value;
        runner->type  = type;
        return;
    }

    runner->next = entry;
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

void printHashmapPrivate (Hashmap* hashmap, uint32_t amount) {
    int firstValue        = 1;
    uint32_t insideAmount = amount + 4;
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
        printHashmapEntry (entry, amount);
        HashmapEntry* runner = entry->next;
        while (runner != NULL) {
            printf (",\n");
            printSpaces (insideAmount);
            printHashmapEntry (runner, amount);
            runner = runner->next;
        }
    }
    printf ("\n");
    printSpaces (amount);
    printf ("}");
}

void printHashmapEntry (HashmapEntry* entry, uint32_t amount) {
    printSuperPrimitive (entry->key);
    printf (":");
    printf (" ");
    switch (entry->type) {
    case SUPER_PRIMITIVE:
        printSuperPrimitive ((SuperPrimitive*)entry->value);
        break;
    case HASHMAP:
        printHashmapPrivate ((Hashmap*)entry->value, amount + 4);
        break;
    case LIST: break;
    }
}
