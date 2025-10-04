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
typedef enum HashmapReturnCodes { SUCCESS, KEY_NOT_FOUND } HashmapReturnCodes;
typedef struct HashmapEntry {
    void* value;
    SuperPrimitive* key;
    HashmapEntryType type;
    uint32_t hashOriginal;
    struct HashmapEntry* next;
} HashmapEntry;
typedef struct Hashmap {
    HashmapEntry** entries;
    uint32_t length;
} Hashmap;
Hashmap* createHashmap ();
HashmapReturnCodes deleteEntryByKey (Hashmap* hashmap, SuperPrimitive* key);
void freeHashmap (Hashmap* hashmap);
void setKeyAndValue (Hashmap* hashmap, SuperPrimitive* key, void* value, HashmapEntryType type);
void printHashmap (Hashmap* hashmap);
void printHashmapEntry (HashmapEntry* entry);
HashmapReturnCodes
getValueByKey (Hashmap* hashmap, SuperPrimitive* key, HashmapEntry* returnValue);
void printHashmapEntryValue (HashmapEntry* entry);
#endif
