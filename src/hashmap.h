#ifndef __hashmap_h
#define __hashmap_h
#include "superPrimitive.h"
#include <stdint.h>
#define ARRAY_INITIAL_LENGTH 256
typedef enum HashmapEntryType {
    SUPER_PRIMITIVE,
    HASHMAP,
    LIST
} HashmapEntryType;
typedef struct HashmapEntry {
    void* value;
    SuperPrimitive* key;
    HashmapEntryType type;
    struct HashmapEntry* next;
} HashmapEntry;
typedef struct Hashmap {
    HashmapEntry** entries;
    uint32_t length;
} Hashmap;
Hashmap* createHashmap ();
void freeHashmap (Hashmap* hashmap);
void setKeyAndValue (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type);
void printHashmap (Hashmap* hashmap);
#endif
