#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

List* createList () {
    List* list = (List*)(malloc (sizeof (List)));
    return list;
}

void addValueToList (List* list, EntryValue* value) {
    assert (list != NULL);
    assert (value != NULL);
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


void freeList (List* list) {
    assert (list != NULL);
    List* runner   = list;
    List* previous = NULL;
    while (runner != NULL) {
        previous = runner;
        runner   = runner->next;
        free (previous);
    }
}
