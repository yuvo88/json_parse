#include "hashmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void printList (List* list, uint32_t spaceAmount);
List* createList () {
    List* list = (List*)(malloc (sizeof (List)));
    return list;
}

void addValueToList (List* list, EntryValue* value) {
    if (list->value == NULL) {
        list->value = value;
        return;
    }

    List* newNode  = (List*)(malloc (sizeof (List)));
    newNode->value = value;
    List* runner   = list;
    while (runner->next != NULL) {
        runner = runner->next;
    }
    runner->next = newNode;
}

void printListln (List* list) {
    printList (list, 0);
    printf ("\n");
}
void printList (List* list, uint32_t spaceAmount) {
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
        printEntryValue (runner->value, insideAmount);
        runner = runner->next;
    }

    printf ("\n");
    printSpaces (spaceAmount);
    printf ("]");
}

void freeList (List* list) {
    List* runner   = list;
    List* previous = NULL;
    while (runner != NULL) {
        freeEntryValue (runner->value);
        previous = runner;
        runner   = runner->next;
        free (previous);
    }
}
