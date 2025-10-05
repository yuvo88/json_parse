#include "hashmap.h"
#include <assert.h>
#include <stdio.h>


void printHashmapln (Hashmap* hashmap) {
    assert(hashmap != NULL);
    printHashmap (hashmap, 0);
    printf ("\n");
}

void printSpaces (uint32_t amount) {
    for (uint32_t i = 0; i < amount; i++) {
        printf (" ");
    }
}

void printHashmapEntryln (HashmapEntry* entry) {
    assert(entry != NULL);
    printHashmapEntry (entry, 0);
    printf ("\n");
}

void printHashmapEntryValueln (HashmapEntry* entry) {
    assert(entry != NULL);
    printHashmapEntryValue (entry, 0);
    printf ("\n");
}

void printHashmap (Hashmap* hashmap, uint32_t spaceAmount) {
    assert(hashmap != NULL);
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
        printHashmapEntry (entry, spaceAmount);
        HashmapEntry* runner = entry->next;
        while (runner != NULL) {
            printf (",\n");
            printSpaces (insideAmount);
            printHashmapEntry (runner, spaceAmount);
            runner = runner->next;
        }
    }
    printf ("\n");
    printSpaces (spaceAmount);
    printf ("}");
}

void printHashmapEntryValue (HashmapEntry* entry, uint32_t spaceAmount) {
    assert(entry != NULL);
    switch (entry->type) {
    case SUPER_PRIMITIVE:
        printSuperPrimitive ((SuperPrimitive*)entry->value);
        break;
    case HASHMAP: printHashmap ((Hashmap*)entry->value, spaceAmount + 4); break;
    case LIST: break;
    }
}

void printHashmapEntry (HashmapEntry* entry, uint32_t spaceAmount) {
    assert(entry != NULL);
    printSuperPrimitive (entry->key);
    printf (":");
    printf (" ");
    printHashmapEntryValue (entry, spaceAmount);
}
