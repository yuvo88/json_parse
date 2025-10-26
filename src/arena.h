#ifndef __arena_h
#define __arena_h
#include <stdint.h>
typedef struct FreeList {
    uint64_t size;
    void* pointer;
    struct FreeList* next;
} FreeList;
typedef struct Arena {
    uint64_t size;
    uint64_t pointer;
    uint8_t* memory;
    FreeList* freeList;
} Arena;
Arena* createArena (uint64_t size);
void destroyArena (Arena* arena);
void* arenaMalloc (Arena* arena, uint64_t size);
void freeArenaItem (Arena* arena, void* pointer, uint64_t size);
void* arenaCalloc (Arena* arena, uint64_t size);
void* arenaRealloc (Arena* arena, void* pointer, uint64_t oldSize, uint64_t newSize);
#endif
