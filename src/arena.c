#include "arena.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Arena* createArena (uint64_t size) {
    Arena* arena = (Arena*)malloc (sizeof (Arena));
    if (arena == NULL) {
        return NULL;
    }
    arena->memory = (uint8_t*)calloc (1, size);
    if (arena->memory == NULL) {
        free (arena);
        return NULL;
    }
    arena->size     = size;
    arena->pointer  = 0;
    arena->freeList = NULL;
    return arena;
}
void destroyArena (Arena* arena) {
    assert (arena != NULL);
    FreeList* runner   = arena->freeList;
    FreeList* previous = NULL;
    while (runner != NULL) {
        previous = runner;
        runner   = runner->next;
        free (previous);
    }

    free (arena->memory);
    free (arena);
}
void* arenaRealloc(Arena* arena, void* pointer, uint64_t oldSize, uint64_t newSize) {
    void* newPointer = arenaMalloc(arena, newSize);
    memcpy(newPointer, pointer, oldSize);
    freeArenaItem(arena, pointer, oldSize);

    return newPointer;
}
void* arenaCalloc(Arena* arena, uint64_t size) {
    void* pointer = (uint8_t*)arenaMalloc(arena, size);
    memset(pointer, 0, size);
    return pointer;
    
}
void* arenaMalloc (Arena* arena, uint64_t size) {
    assert (arena != NULL);
    if (arena->pointer + size > arena->size) {
        return NULL;
    }
    if (size == 0) {
        return NULL;
    }

    FreeList* runner = arena->freeList;
    while (runner != NULL && runner->size < size) {
        runner = runner->next;
    }

    if (runner == NULL) {
        uint64_t oldPointer = arena->pointer;
        arena->pointer += size;
        return arena->memory + oldPointer;
    }

    runner->size -= size;
    void* oldPointer = runner->pointer;
    runner->pointer  = oldPointer + size;

    return oldPointer;
}
void freeArenaItem (Arena* arena, void* pointer, uint64_t size) {
    assert (arena != NULL);
    assert (pointer != NULL);

    if (size == 0) {
        return;
    }

    if (arena->freeList == NULL) {
        arena->freeList          = (FreeList*)malloc (sizeof (FreeList));
        arena->freeList->size    = size;
        arena->freeList->pointer = pointer;
        return;
    }

    FreeList* runner = arena->freeList;
    while (runner->next != NULL) {
        runner = runner->next;
    }

    runner->next          = (FreeList*)malloc (sizeof (FreeList));
    runner->next->size    = size;
    runner->next->pointer = pointer;
}
