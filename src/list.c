#include "arena.h"
#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

List* createList (Arena* arena) {
    List* list = (List*)(arenaMalloc (arena, sizeof (List)));
    return list;
}

void addValueToList (Arena* arena, List* list, EntryValue* value) {
    assert(arena != NULL);
    assert (list != NULL);
    assert (value != NULL);
    if (list->value == NULL) {
        list->value = value;
        return;
    }

    List* newNode  = (List*)(arenaCalloc (arena,sizeof (List)));
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
