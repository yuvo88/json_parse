#include "hashmap.h"
#include "superPrimitive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void printList (List* list, uint32_t spaceAmount);
List* createList () {
    List* list = (List*)(malloc (sizeof (List)));
    return list;
}

void addValueToList (List* list, void* value, EntryType type) {
    List* newNode  = (List*)(malloc (sizeof (List)));
    newNode->value = value;
    newNode->type  = type;
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
        } else {
            firstValue = 0;
        }
        printSpaces (insideAmount);
        switch (runner->type) {
        case LIST: printList ((List*)runner->value, insideAmount); break;
        case HASHMAP:
            printHashmap ((Hashmap*)runner->value, insideAmount);
            break;
        case SUPER_PRIMITIVE:
            printSuperPrimitive ((SuperPrimitive*)runner->value);
            break;
        }
        printf ("\n");
        runner = runner->next;
    }
    printSpaces (spaceAmount);
    printf ("]");
}

void freeList (List* list) {
    List* runner   = list;
    List* previous = NULL;
    while (runner != NULL) {
        switch (runner->type) {
        case LIST: freeList ((List*)runner->value); break;
        case HASHMAP: freeHashmap ((Hashmap*)runner->value); break;
        case SUPER_PRIMITIVE:
            freeSuperPrimitive ((SuperPrimitive*)runner->value);
            break;
        }
        previous = runner;
        runner   = runner->next;
        free (previous);
    }
}
