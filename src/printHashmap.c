#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>


void printHashmapln (Hashmap* hashmap) {
    assert (hashmap != NULL);
    printHashmap (hashmap, 0);
    printf ("\n");
}

void printSpaces (uint32_t amount) {
    for (uint32_t i = 0; i < amount; i++) {
        printf (" ");
    }
}

void printHashmapEntryln (HashmapEntry* entry) {
    assert (entry != NULL);
    printHashmapEntry (entry, 0);
    printf ("\n");
}


void printHashmap (Hashmap* hashmap, uint32_t spaceAmount) {
    assert (hashmap != NULL);
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


void printHashmapEntry (HashmapEntry* entry, uint32_t spaceAmount) {
    assert (entry != NULL);
    printSuperPrimitive (entry->key);
    printf (":");
    printf (" ");
    printEntryValue (entry->value, spaceAmount);
}

void printEntryValueln (EntryValue* entryValue) {
    assert (entryValue != NULL);
    printEntryValue (entryValue, 0);
    printf ("\n");
}

void printEntryValue (EntryValue* entryValue, uint32_t spaceAmount) {
    assert (entryValue != NULL);
    uint32_t insideAmount = spaceAmount + 4;
    switch (entryValue->type) {
    case SUPER_PRIMITIVE:
        printSuperPrimitive ((SuperPrimitive*)entryValue->value);
        break;
    case HASHMAP:
        printHashmap ((Hashmap*)entryValue->value, insideAmount);
        break;
    case LIST: printList ((List*)entryValue->value, insideAmount);
               break;
    case NULL_VALUE:
               printf("null");
               break;
    }
}

void printListln (List* list) {
    assert (list != NULL);
    printList (list, 0);
    printf ("\n");
}
void printList (List* list, uint32_t spaceAmount) {
    assert (list != NULL);
    uint32_t insideAmount = spaceAmount + 4;
    int firstValue        = 1;
    List* runner          = list;
    printf ("[");
    printf ("\n");
    while (runner != NULL) {
        if (!firstValue) {
            printf (",");
            printf ("\n");
        } else {
            firstValue = 0;
        }
        printSpaces (insideAmount);
        printEntryValue (runner->value, spaceAmount);
        runner = runner->next;
    }

    printf ("\n");
    printSpaces (spaceAmount);
    printf ("]");
}
